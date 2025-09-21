package p1.server;

import utils.*;

import java.io.IOException;
import java.util.Random;

/**
 * Class that implements the Battleship game logic.
 */
public class BattleshipGame implements IBattleshipGame{


    /**
     * Communication utility for handling input and output streams.
     */
    BattleshipComUtils bsComUtils;

    /**
     * Game object that contains the game information.
     */
    private Game game;

    /**
     * Save the error code to send later in addvessel
     */
    private ErrorCode errorCode;

    public BattleshipGame(BattleshipComUtils bsComUtils) {
        this.bsComUtils = bsComUtils;
    }

    public void setGame(Game game) {
        this.game = game;
    }

    public Game getGame() {
        return game;
    }

    public ErrorCode getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(ErrorCode errorCode) {
        this.errorCode = errorCode;
    }

    public BattleshipComUtils getBsComUtils() {
        return bsComUtils;
    }


    /**
     * Gets the ID of the active player.
     *
     * @return the ID of the player whose turn it is.
     */
    @Override
    public int getActivePlayer() {
        if (getGameState() == 4) { //If FINISHED state
            System.out.println("Game is finished , no one is active player");
            return -1;
        }

        if (game.getGameInfo().tornJugador1 == 1 && game.getGameInfo().tornJugador2 == 1 || game.getGameInfo().tornJugador1 == 0 && game.getGameInfo().tornJugador2 == 0) {
            System.out.println("ERROR: Both players have the same turn (tornJugador1 and tornJugador2)");
            return -1;


        } else {
            if (game.getGameInfo().tornJugador1 == 1) {
                return game.getPlayer1ID();
            } else {
                return game.getPlayer2ID();
            }
        }
    }

    /**
     * Gets the current game state.
     *
     * @return an integer representing the game state (see gameState description).
     */
    @Override
    public int getGameState() {
        return game.getGameState().getCode();
    }

    /**
     * Gets the playerId of the winning player if the game has ended.
     *
     * @return the playerId of the winning player, or -1 if the game is still ongoing.
     */
    @Override
    public int getWinPlayer() {

        if (getActivePlayer() == game.getPlayer1ID()){
            System.out.println("Server : Player 1 is active player, checking if player 1 has won");
            if (game.getGameInfo().hasThePlayerWon(1)) {
                System.out.println("Server : Player 1 has won the game");
                game.getGameInfo().setGuanyaJugador1((byte)1);
                game.getGameInfo().setGuanyaJugador2((byte)0);
                return game.getPlayer1ID();
            }
        } else if (getActivePlayer() == game.getPlayer2ID()) {
            System.out.println("Server : Player 2 is active player, checking if player 2 has won");
            if (game.getGameInfo().hasThePlayerWon(2)) {
                System.out.println("Server : Player 2 has won the game");
                game.getGameInfo().setGuanyaJugador1((byte)0);
                game.getGameInfo().setGuanyaJugador2((byte)1);
                return game.getPlayer2ID(); // return -1 (bot player)
            }
        } else{
            System.out.println("Server : No active player???");
            return -999;
        }
        return -999; // No winner yet
    }

    /**
     * Gets the number of remaining vessels of a specific type for a given player.
     *
     * @param playerId the ID of the player.
     * @param type     the type of vessel.
     * @return the number of remaining vessels of the specified type.
     */
    @Override
    public int getRemainingVessels(int playerId, int type) {
        return game.getGameInfo().getNShipsRemaining(type);
    }

    /**
     * Gets the total number of players in the game.
     *
     * @return the number of players.
     */
    @Override
    public int getNumPlayers() {
        return 1;
    }

    /**
     * Gets the player ID at the specified index.
     *
     * @param idx the index of the player.
     * @return the player ID corresponding to the given index.
     */
    @Override
    public int getPlayerId(int idx) {
        if (idx == 0) {
            return game.getPlayer1ID();
        } else if (idx == 1) {
            return game.getPlayer2ID();
        }
        return -1;
    }

    /**
     * Checks whether a player is ready to start the game.
     *
     * @param playerId the ID of the player.
     * @return true if the player is ready, false otherwise.
     */
    @Override
    public boolean isPlayerReady(int playerId) {
        return false;
    }

    /**
     * Attempts to add a vessel to the board for a given player.
     *
     * @param playerId the ID of the player.
     * @param ri       the starting row index.
     * @param ci       the starting column index.
     * @param rf       the ending row index.
     * @param cf       the ending column index.
     * @param typeINT  the type of vessel.
     * @return true if the vessel was successfully placed, false otherwise.
     */
    @Override
    public boolean addVessel(int playerId, int ri, int ci, int rf, int cf, int typeINT) {

        if (game.getGameInfo().getGameState() == CurrentState.SETUP) {
            if (checkVessel((byte) typeINT, (byte) ri, (byte) ci, (byte) rf, (byte) cf)) {
                return changeGameBoard((byte) ri, (byte) ci, (byte) rf, (byte) cf, (byte) typeINT, playerId == game.getPlayer1ID());
            } else {
                System.out.println("ERROR: Vessel not added");
                //Error code in checkVessel
                return false;
            }
        } else { // wrong state
            System.out.println("ERROR: Wrong state (should be setup and not -> " + game.getGameInfo().getGameState() + " )");
            errorCode = ErrorCode.INVALID_STATE;
            return false;
        }
    }

    /**
     * Checks whether a player is a bot.
     *
     * @param playerId the ID of the player.
     * @return true if the player is a bot, false otherwise.
     */
    @Override
    public boolean isBot(int playerId) {

        if (game.getGameState()== CurrentState.SETUP || game.getGameState() == CurrentState.FINISHED) {
            System.out.println("ERROR: Game not in PLAYING state (isBot)");
            return false;
        }

        return game.getPlayer2ID() == playerId; // Player 2 is always a bot
    }

    /**
     * Processes a shot fired by a player at a specific location.
     *
     * @param playerId the ID of the player taking the shot.
     * @param r        the row index of the shot.
     * @param c        the column index of the shot.
     * @return an integer representing the result of the shot: miss (0), hit(1), sunk(2). -1 in case of error.
     */
    @Override
    public int shot(int playerId, int r, int c) {

        // Verify r and c values
        if (r < 0 || r >= game.getGameInfo().H || c < 0 || c >= game.getGameInfo().W) {
            System.out.println("ERROR: Invalid shot coordinates");
            errorCode = ErrorCode.INVALID_COORDINATE;
            return -1; // ERROR
        }

        if (playerId == game.getPlayer1ID()) {
            // player ID shots to player 2 board

            byte valueBoard = game.getGameInfo().getBoard2Coords(r, c); // byte as nnt (n = ship instance, t = type of ship)
            int n = valueBoard / 10; // nn instance (can be negative)
            int t = Math.abs(valueBoard % 10); // t type (abs to avoid negative values)

            if (t == 0) {

                if (n == 0) {
                    // change n to 1 ( as it is empty with a missed shot )
                    game.getGameInfo().setBoard2Coords(r, c, (byte) 10); // Fail value (010)
                } else if (n > 3) {
                    System.out.println("ERROR: Not possible value???");
                }
                return 0; // FAIL

            } else if (t >= 1 && t <= 5 && n != 0) {

                System.out.println("r,c,n,t -> " + r + "," + c + "," + n + "," + t);

                if (game.getGameInfo().isLastInstanceOfShip(2, r, c, n, t)) {

                    System.out.println("LAST INSTANCE FOUND PLAYER 1, SWAPING TO SUNK instance --> " + n + ", type --> " + ShipType.fromCode((byte) t) + " and playerId --> " + playerId);
                    changeAllInstances(n, t, playerId); // Mark all instances of the ship as SUNK on the board
                    game.getGameInfo().setBoard2Coords(r, c, (byte) 30); // Sunk value (030)
                    return 2; // SUNK
                } else {
                    game.getGameInfo().setBoard2Coords(r, c, (byte) 20); // Hit value (020)
                    return 1; // HIT
                }


            } else {
                System.out.println("ERROR: Invalid ship type");
                errorCode = ErrorCode.TYPE_UNAVAILABLE;
                return -1; // ERROR
            }

        } else if (playerId == game.getPlayer2ID()) {

            // player ID shots to player 1 board

            byte valueBoard = game.getGameInfo().getBoard1Coords(r, c); // byte as nnt (n = ship instance, t = type of ship)
            int n = valueBoard / 10; // nn instance
            int t = Math.abs(valueBoard % 10); // t type (abs to avoid negative values)

            if (t == 0) {

                if (n == 0) {
                    // change n to 1 ( as it is now empty with a missed shot )
                    game.getGameInfo().setBoard1Coords(r, c, (byte) 10); // Fail value (010)
                } else if (n > 3) {
                    System.out.println("ERROR: Not possible value");
                }
                return 0; // FAIL

            } else if (t >= 1 && t <= 5 && n != 0) {

                if (game.getGameInfo().isLastInstanceOfShip(1, r, c, n, t)) {

                    System.out.println("LAST INSTANCE FOUND PLAYER2, SWAPING TO SUNK n --> " + n + " playerId --> " + playerId);
                    changeAllInstances(n, t ,playerId); // Mark all instances of the ship as SUNK on the board
                    game.getGameInfo().setBoard1Coords(r, c, (byte) 30); // Sunk value (030)
                    return 2; // SUNK
                } else {
                    game.getGameInfo().setBoard1Coords(r, c, (byte) 20); // Hit value (020)
                    return 1; // HIT
                }


            } else {
                System.out.println("ERROR: Invalid ship type");
                errorCode = ErrorCode.TYPE_UNAVAILABLE;
                return -1; // ERROR
            }

        } else {
            System.out.println("ERROR: Invalid player ID");
            errorCode = ErrorCode.INVALID_PLAYER_ID;
            return -1; // ERROR
        }
    }
    /**
     * Processes an automatic shot for a bot player.
     *
     * @param playerId the ID of the bot player.
     * @return an integer representing the result of the shot: miss (0), hit(1), sunk(2). -1 in case of error.
     */
    @Override
    public int shot(int playerId) {

        if (isBot(playerId)) {  // bot shot

            System.out.println("------------------ BOT TURN ( real playerID game --> " + game.getPlayer1ID() + ")------------------");
            int contador = 0;
            while (true) {
                Random random = new Random();

                int r = random.nextInt(game.getGameInfo().H);
                int c = random.nextInt(game.getGameInfo().W);

                int result = shot(getActivePlayer(), r, c);

                System.out.println("Server : Bot shot (0-based) --> (" + r + "," + c + ")" + "||  (1-based) --> (" + (r + 1) + "," + (c + 1) + ")");

                if (result == -1) { // should never happen
                    System.out.println("Server : Bot shot error?????");
                    continue;
                }

                if (result == 0) { // FAIL
                    System.out.println("Server : Bot FAIL ");
                    // ending bot shot
                    game.getGameInfo().tornJugador1 = game.getGameInfo().tornJugador1 == 1 ? (byte) 0 : (byte) 1;
                    game.getGameInfo().tornJugador2 = game.getGameInfo().tornJugador2 == 1 ? (byte) 0 : (byte) 1;
                    break;
                } else if (result == 1) { // HIT
                    System.out.println("Server : Bot HIT ");
                    contador++;
                    this.notifyStatus(getPlayerId(0)); // notify the player the hit
                    continue;
                } else if (result == 2) {
                    System.out.println("Server : Bot SUNK ");
                    contador++;
                    int winner = getWinPlayer();
                    if (winner != -999) {
                        return 2; // end game
                    }
                    this.notifyStatus(getPlayerId(0));
                }
            }

            System.out.println("------------------ BOT TURN ENDED (hits --> " + contador + ") ------------------");

            this.notifyStatus(getPlayerId(0));
            return 0;

        } else {
            System.out.println("ERROR: Invalid player ID, isBot said that the bot is not a bot. ");
            return -1;
        }
    }

    /**
     * Notifies the game status to a given player.
     *
     * @param playerId the ID of the player.
     * @return true if the status notification was successful, false otherwise.
     */
    @Override
    public boolean notifyStatus(int playerId) {

        try {
            //this.printStatus(); //Print the current game status

            if (getGameState() == 4) { //If FINISHED state

                System.out.println("SERVER : Game is finished , sending the complete board of both players");

                System.out.println("Print del get info que envio");
                printStatus();

                bsComUtils.write_GAMESTATUS((byte) getGameState(),
                        game.getGameInfo().boardSize,
                        game.getGameInfo().boardPlayer1,
                        game.getGameInfo().boardPlayer2, // send all the board
                        game.getGameInfo().getInfo(game.getGameState()));

                return true;
            }

            if (playerId == game.getPlayer1ID()) {


                bsComUtils.write_GAMESTATUS((byte) getGameState(),
                        game.getGameInfo().boardSize,
                        game.getGameInfo().boardPlayer1,
                        hideShips(game.getGameInfo().boardPlayer2), // Hide ships from player 2
                        game.getGameInfo().getInfo(game.getGameState()));
            } else {
                throw new GameException("ERROR : NO PLAYER 2 IMPLEMENTED, SHOULDN'T BE HERE");
            }
            return true;

        } catch (Exception e) {
            System.out.println("Server (notifyStatus) : Error notifying status: " + e.getMessage());
            return false;
        }
    }
    /**
     * Player leave the game.
     *
     * @param playerId the ID of the player.
     */
    @Override
    public void leaveGame(int playerId) {
        game.setGameState(CurrentState.FINISHED); // Set game state to FINISHED
    }

    /**
     * End the game by setting the game state to FINISHED.
     */
    @Override
    public void endGame() throws IOException {
        game.setGameState(CurrentState.FINISHED); //Change state to finished
        System.out.println("Server : Game state FINISHED"); //Notify
    }


    /**
     * Prints the current game status.
     */
    public void printStatus() {

        // info sent through the GAMESTATUS message
        System.out.println("Server (notifyStatus) : Notifying status to player: ");
        System.out.println("Server (notifyStatus) : Game state: " + getGameState());
        System.out.println("Server (notifyStatus) : Board size: " + game.getGameInfo().boardSize);
        System.out.println("Server (notifyStatus) : Board 1 and Board 2 ");
        game.getGameInfo().printBoard1();
        game.getGameInfo().printBoard2();
        System.out.println("Server (notifyStatus) : Info");
        game.getGameInfo().printInfo(game.getGameState());
    }

    /**
     * Check if the vessel can be placed in the given coordinates
     * First condition is to check the type of the vessel
     * Then check if the coordinates are within bounds
     * Then check if the vessel is horizontal or vertical
     * Then check if the vessel has the correct size
     * Then check if the vessel is placed in a empty position
     *
     * @param type type of the ship
     * @param ri  row initial
     * @param ci column initial
     * @param rf row final
     * @param cf column final
     * @return true if the vessel can be placed, false otherwise
     */
    public boolean checkVessel(byte type, byte ri, byte ci, byte rf, byte cf) {

        ri--;
        ci--;
        rf--;
        cf--; // Adjust the coordinates to 0-based

        // incorrect type
        if (type < 1 || type > 5) {
            System.out.println("ERROR : Tipus de vaixell incorrecte, ha de ser entre 1 i 5 (ambdos inclosos). Vaixell rebut: " + type);
            errorCode = ErrorCode.TYPE_UNAVAILABLE;
            return false;
        }
        // Check if the coordinates are within bounds after adjusting (coordinates 0-based)
        // first check if the coordinates don't have negative values
        // then check that the last position is within the board
        if (ri < 0 || ci < 0 || rf < 0 || cf < 0 || rf >= game.getGameInfo().H || cf >= game.getGameInfo().W) {

            if (ri < 0 || ci < 0 || rf < 0 || cf < 0) {
                System.out.println("ERROR: Les coordenades no poden ser negatives");
            } else {
                System.out.println("ERROR: El vaixell surt fora del tauler!");
            }
            errorCode = ErrorCode.INVALID_COORDINATE;
            return false;
        }
        // incorrect coordinates not in the same row or column
        if (ri != rf && ci != cf) {
            System.out.println("ERROR : El vaixell ha de ser horitzontal o vertical");
            errorCode = ErrorCode.INVALID_COORDINATE;
            return false;
        }

        int amplada = Math.abs(cf - ci) + 1;  // Number of columns occupied
        int altura = Math.abs(rf - ri) + 1;   // Number of rows occupied

        // Incorrect size
        if (amplada != ShipType.typeToSize(type) && altura != ShipType.typeToSize(type)) {
            System.out.println("ERROR : El vaixell no té la mida correcta");

            System.out.println("La mida correcta és: " + ShipType.typeToSize(type));
            System.out.print("La mida actual és : ");

            if (ri == rf) {
                System.out.println(amplada);
            } else {
                System.out.println(altura);
            }
            errorCode = ErrorCode.INVALID_LENGTH;
            return false;
        }


        // check if the vessel is placed in a empty position
        for (int i = Math.min(ri, rf); i <= Math.max(ri, rf); i++) {
            for (int j = Math.min(ci, cf); j <= Math.max(ci, cf); j++) {
                if (game.getGameInfo().getBoard1Coords(i, j) != 0) {
                    System.out.println("ERROR: La posició :" + i + "," + j + " està ocupada");
                    errorCode = ErrorCode.INVALID_COORDINATE;
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Change the game board with the given coordinates
     * Shouldn't get here if the coordinates are invalid
     * @param ri row initial
     * @param ci column initial
     * @param rf row final
     * @param cf column final
     * @param type type of the ship
     * @param isPlayer1 true if the player is player 1, false otherwise
     * @return true if the vessel was successfully placed, false otherwise.
     */
    public boolean changeGameBoard(byte ri, byte ci, byte rf, byte cf, byte type, boolean isPlayer1) {
        // Place the ship on the board
        ri--;
        ci--;
        rf--;
        cf--; // Adjust the coordinates to 0-based

        // Determine the instance number of the ship (1-25) (it is a byte, so it can be negative (-12)-12 )
        byte instanceNumber = game.getGameInfo().getInstanceAndIncrease(type);

        if (instanceNumber > 25) {
            System.out.println("ERROR: Too many ships");
            return false;
        }

        byte encodedValue;

        // encode the value

        // first check if the instance number is greater than 12
        // if its not negative, encode the value normally

        // to encode the value, we need to multiply the instance number by 100 so we get the n value (from nnt)
        // then we get the second n from the instance number by getting the remainder of the division by 10
        // then we get the type of the ship and we add it as the unit of the number

        // if its negative we do the same but we need to get the negative value
        // but before that we need to ensure that the instance number is not greater than 13, as the byte can't hold a 13


        if (instanceNumber > 12) { // negative instance number (can't put a 13t in a byte)

            byte valor = 25; // total possible ships
            byte instance = (byte) (valor - instanceNumber); // negative next instance number

            // Encode the normal value
            encodedValue = (byte) ((instance / 10) * 100 + (instance % 10) * 10 + type);

            // get the negative value
            encodedValue = (byte) (encodedValue * -1);

        } else {
            encodedValue =  (byte) ((instanceNumber / 10) * 100 + (instanceNumber % 10) * 10 + type);
        }

        // Horizontal ship: advance by columns
        if (ri == rf) {
            for (int j = ci; j <= cf; j++) {
                if (isPlayer1) {
                    game.getGameInfo().setBoard1Coords(ri, j, encodedValue);
                } else {
                    game.getGameInfo().setBoard2Coords(ri, j, encodedValue);
                }
            }
        }
        // Vertical ship: advance by rows
        else {
            for (int i = ri; i <= rf; i++) {
                if (isPlayer1) {
                    game.getGameInfo().setBoard1Coords(i, ci, encodedValue);
                } else {
                    game.getGameInfo().setBoard2Coords(i, ci, encodedValue);
                }
            }
        }

        // Update the number of remaining ships of this type
        game.getGameInfo().decreaseNShips(type);

        return true;
    }


    /**
     * Censore the board to hide the ships, and show only the hits and misses
     * @param board the board to censore
     * @return the censored board
     */
    private byte[] hideShips(byte[] board){

        byte[] hiddenBoard = new byte[board.length];
        for (int i = 0; i < board.length; i++) {

            if (board[i] == 10) {
                hiddenBoard[i] = 1;
            } else if (board[i] == 20) {
                hiddenBoard[i] = 2;
            } else if (board[i] == 30) {
                hiddenBoard[i] = 3;
            } else {
                hiddenBoard[i] = 0;
            }

        }
        return hiddenBoard;
    }

    /**
     * Print the current game
     */
    public void printGame(){
        System.out.println("--------------------");
        System.out.println("Current player: " + getActivePlayer());
        System.out.println("Board player 1");
        game.getGameInfo().printBoard1();
        System.out.println("Board player 2");
        game.getGameInfo().printBoard2();
        System.out.println("--------------------");
    }

    /**
     * Change from hit to sunk all the instances of the ship
     * @param n instance of the ship that is sunk
     * @param playerId player that sunk the ship
     */
    public void changeAllInstances(int n, int t, int playerId) {

        System.out.println("Changing all instances of ship " + n + " and type " + t  + " to SUNK");

        if (n<0){
            System.out.println("Change All instances : the type is negative, changing to positive");
            t = Math.abs(t);
        }

        if (playerId == game.getPlayer1ID()) {
            for (int i = 0; i < game.getGameInfo().boardSize; i++) {
                if (game.getOriginalBoard2()[i] / 10 == n ) { // no need to check if the type is the same, as the type is the same for all the instances and the vessel is sunk
                    System.out.println("Changing instance " + n + " of type " + t + " to SUNK in position " + i + " (player 2)");
                    game.getGameInfo().boardPlayer2[i] = 30; //Change to SUNK(030)
                }
            }
        } else {
            for (int i = 0; i < game.getGameInfo().boardSize; i++) {
                if (game.getOriginalBoard1()[i] / 10 == n ) {
                    game.getGameInfo().boardPlayer1[i] = 30; //Change to SUNK(030)
                }
            }
        }
    }


    /**
     * Initialize board2 randomly with different number of ships for each type
     * @param shipsRemainingType1 number of ships of type 1
     * @param shipsRemainingType2 number of ships of type 2
     * @param shipsRemainingType3 number of ships of type 3
     * @param shipsRemainingType4 number of ships of type 4
     * @param shipsRemainingType5 number of ships of type 5
     */
    public void randomBoard2(int shipsRemainingType1, int shipsRemainingType2, int shipsRemainingType3, int shipsRemainingType4, int shipsRemainingType5) throws GameException {
        Random rand = new Random();
        int maxTries = 10000;

        if (shipsRemainingType1 > 25 || shipsRemainingType2 > 25 || shipsRemainingType3 > 25 || shipsRemainingType4 > 25 || shipsRemainingType5 > 25) {
            throw new GameException("Too many ships");
        }


        for (int type = 1; type <= 5; type++) {
            int shipsRemaining;
            byte totalShips= 1; //start with instance 1

            if (type == 1) shipsRemaining = shipsRemainingType1;
            else if (type == 2) shipsRemaining = shipsRemainingType2;
            else if (type == 3) shipsRemaining = shipsRemainingType3;
            else if (type == 4) shipsRemaining = shipsRemainingType4;
            else shipsRemaining = shipsRemainingType5;

            while (shipsRemaining > 0) {
                boolean correcte = false;
                while (!correcte) {
                    boolean horizontal = rand.nextBoolean(); //Horitzontal o vertical
                    int ri, ci, rf, cf;

                    if (horizontal) {
                        ri = rand.nextInt(getGame().getGameInfo().H);
                        ci = rand.nextInt(getGame().getGameInfo().W - ShipType.typeToSize( (byte) type) + 1);
                        rf = ri;
                        cf = ci + ShipType.typeToSize( (byte) type) - 1;
                    } else {
                        ri = rand.nextInt(getGame().getGameInfo().H - ShipType.typeToSize( (byte) type) + 1);
                        ci = rand.nextInt(getGame().getGameInfo().W);
                        rf = ri + ShipType.typeToSize( (byte) type) - 1;
                        cf = ci;
                    }

                    if (checkVesselRandom((byte) type, (byte) ri, (byte) ci, (byte) rf, (byte) cf)) {
                        correcte = changeGameBoard2((byte) ri, (byte) ci, (byte) rf, (byte) cf, (byte) type, (byte) totalShips );
                        //System.out.println("Placed ship type " + type + " at (" + ri + "," + ci + ") to (" + rf + "," + cf + ")");
                    } else {
                        maxTries--;
                        if (maxTries == 0) { // too many tries (board too small or too many ships)
                            System.out.println("ERROR: Couldn't place all ships");
                            throw new GameException("Couldn't place all ships, board too small or too many ships");
                        }
                    }
                }

                totalShips++;
                shipsRemaining--;
            }
        }
    }

    /**
     * Check if the vessel can be placed in the given coordinates
     * First condition is to check the type of the vessel
     * Then check if the coordinates are within bounds
     * Then check if the vessel is horizontal or vertical
     * Then check if the vessel has the correct size
     * Then check if the vessel is placed in a empty position
     * @param type type of the ship
     * @param ri row initial
     * @param ci column initial
     * @param rf row final
     * @param cf column final
     * @return true if the vessel can be placed, false otherwise
     */

    public boolean checkVesselRandom(byte type, byte ri, byte ci, byte rf, byte cf) {

        // incorrect type
        if (type < 1 || type > 5) {
            return false;
        }
        // Check if the coordinates are within bounds after adjusting (coordinates 0-based)
        // first check if the coordinates don't have negative values
        // then check that the last position is within the board
        if (ri < 0 || ci < 0 || rf < 0 || cf < 0 || rf >= getGame().getGameInfo().H || cf >= getGame().getGameInfo().W) {
            return false;
        }
        // incorrect coordinates not in the same row or column
        if (ri != rf && ci != cf) {
            return false;
        }

        int amplada = Math.abs(cf - ci) + 1;  // Number of columns occupied
        int altura = Math.abs(rf - ri) + 1;   // Number of rows occupied

        // Incorrect size
        if (amplada != ShipType.typeToSize(type) && altura != ShipType.typeToSize(type)) {
            return false;
        }

        // check if the vessel is placed in a empty position
        for (int i = Math.min(ri, rf); i <= Math.max(ri, rf); i++) {
            for (int j = Math.min(ci, cf); j <= Math.max(ci, cf); j++) {
                if (getGame().getGameInfo().getBoard2Coords(i, j) != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Change the game board 2 with the random given coordinates
     * Shouldn't get here if the coordinates are invalid
     * @param ri start row
     * @param ci start column
     * @param rf end row
     * @param cf end column
     * @param type type of the ship
     * @param instanceNumber instance number of the ship

     * @return true if the vessel was successfully placed, false otherwise.
     */
    private boolean changeGameBoard2(byte ri, byte ci, byte rf, byte cf, byte type, byte instanceNumber) throws GameException {
        // Place the ship on the board

        if (instanceNumber > 25) {
            throw new GameException("Too many ships");
        }

        byte encodedValue;

        // encode the value

        // first check if the instance number is greater than 12
        // if its not negative, encode the value normally

        // to encode the value, we need to multiply the instance number by 100 so we get the n value (from nnt)
        // then we get the second n from the instance number by getting the remainder of the division by 10
        // then we get the type of the ship and we add it as the unit of the number

        // if its negative we do the same but we need to get the negative value
        // but before that we need to ensure that the instance number is not greater than 13, as the byte can't hold a 13


        if (instanceNumber > 12) { // negative instance number (can't put a 13t in a byte)

            byte valor = 25; // total possible ships
            byte instance = (byte) (valor - instanceNumber); // negative next instance number

            // Encode the normal value
            encodedValue = (byte) ((instance / 10) * 100 + (instance % 10) * 10 + type);

            // get the negative value
            encodedValue = (byte) (encodedValue * -1);

        } else {
            encodedValue =  (byte) ((instanceNumber / 10) * 100 + (instanceNumber % 10) * 10 + type);
        }


        // Horizontal ship: advance by columns
        if (ri == rf) {
            for (int j = ci; j <= cf; j++) {
                getGame().getGameInfo().setBoard2Coords(ri, j, encodedValue);
            }
        }
        // Vertical ship: advance by rows
        else {
            for (int i = ri; i <= rf; i++) {
                getGame().getGameInfo().setBoard2Coords(i, ci, encodedValue);
            }
        }

        return true;
    }



}
