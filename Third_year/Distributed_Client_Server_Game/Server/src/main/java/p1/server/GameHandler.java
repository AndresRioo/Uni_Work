package p1.server;

import utils.*;
import utils.Message.*;

import java.io.IOException;
import java.util.Random;

/**
 * Handles the game logic for a connected client.
 * This class is responsible for managing the communication and 
 * executing the game protocol from the server's side.
 */
public class GameHandler extends Thread {

    private BattleshipGame battleShipGame;

    /**
     * Initializes the game handler with a communication utility.
     *
     * @param bsComUtils The communication utility for interacting with the client.
     */
    public GameHandler(BattleshipComUtils bsComUtils) {
        this.battleShipGame = new BattleshipGame(bsComUtils);
    }

    /**
     * Gets the game object.
     * @return the game object.
     */
    public Game getGame() {
        return battleShipGame.getGame();
    }

    /**
     * Gets the error code from the addvessel if something happened.
     * @return the error code from ErrorCode class
     */
    public ErrorCode getErrorCode() {
        return battleShipGame.getErrorCode();
    }

    public BattleshipGame getBattleShipGame() {
        return battleShipGame;
    }

    /**
     * Calls the methods in the correct order to run the game
     */
    public void run() {
        System.out.println("Server : GameHandler running... ");

        try {
            init(); //create and join the player
        } catch (IOException e) {
            System.out.println("IO Error init() --> " + e.getMessage());
            closeConnection();
            return;
        } catch (GameException e) {
            System.out.println("GameException init() --> " + e.getMessage());
            closeConnection();
            System.out.println("Server : Ending game");
            return;
        }


        try {

            setup(); // addvessel

            battleShipGame.getGame().setOriginalBoard1(battleShipGame.getGame().getGameInfo().boardPlayer1.clone());
            battleShipGame.getGame().setOriginalBoard2(battleShipGame.getGame().getGameInfo().boardPlayer2.clone()); // for checking the sunk

            playing(); // shot
            finished(); // leave

        } catch (Exception e) {
            System.out.println("FLUX OF THE GAME BROKEN: " + e.getMessage());
        } finally {
            closeConnection();
        }


    }


    /**
     * Initializes game settings and prepares the game session.
     */
    public void init() throws GameException, IOException {

        System.out.println("Server : Initializing game settings...");

        Random random = new Random();
        int randomPlayerId = random.nextInt(89999) + 10000; // Random number between 10000 and 99999
        //int randomPlayer2Id = random.nextInt(89999) + 10000; // Random number between 10000 and 99999
        int randomGameId = random.nextInt(89999) + 10000; // Random number between 10000 and 99999

        MessageAbstract message = battleShipGame.getBsComUtils().read(); // Read message

        if (message instanceof MessageJoin) { // JOIN message
            System.out.println("Player Joined the game, received JOIN message");
            MessageJoin messageJoin = (MessageJoin) message;

            if ( isNameInvalid( messageJoin.getNomJugador() ) ) {
                System.out.println("Server : Invalid player name");
                battleShipGame.getBsComUtils().write_ERROR(ErrorCode.INVALID_PLAYER_NAME.getErrorId(), ErrorCode.INVALID_PLAYER_NAME.getDescription().length(), ErrorCode.INVALID_PLAYER_NAME.getDescription());
                init(); // try again
                return;
            }

            // Create a new game with the random game ID and player ID
            initGameInfoFIXEDVALUES(randomPlayerId, randomGameId);
            battleShipGame.getGame().setPlayer1Name(messageJoin.getNomJugador());
            // values are fixed if the player joins a game

        } else if (message instanceof MessageCreate) {
            System.out.println("Player Created the game, received CREATE message");
            MessageCreate messageCreate = (MessageCreate) message;

            if ( isNameInvalid( messageCreate.getNomJugador() ) ) {
                System.out.println("Server : Invalid player name");
                battleShipGame.getBsComUtils().write_ERROR(ErrorCode.INVALID_PLAYER_NAME.getErrorId(), ErrorCode.INVALID_PLAYER_NAME.getDescription().length(), ErrorCode.INVALID_PLAYER_NAME.getDescription());
                init(); // try again
                return;
            }

            int esPosible = initGameInfoVARIABLEVALUES(randomPlayerId, randomGameId, messageCreate.getW(), messageCreate.getH(), messageCreate.getT1(), messageCreate.getT2(), messageCreate.getT3(), messageCreate.getT4(), messageCreate.getT5(), messageCreate.getIA() ? 1 : 0);

            if ( esPosible == -1 ){ // check if the game is possible (board size and ships has enough space and makes sense)
                System.out.println("Server : Game not possible");
                battleShipGame.getBsComUtils().write_ERROR(ErrorCode.GAME_UNAVAILABLE.getErrorId(), ErrorCode.GAME_UNAVAILABLE.getDescription().length(), ErrorCode.GAME_UNAVAILABLE.getDescription());
                init();
                return;
            }

            battleShipGame.getGame().setPlayer1Name(messageCreate.getNomJugador());
            //battleShipGame.getGame().setPlayer2AI(messageCreate.getIA()); //not implemented, change if implemented (player 2 is always a bot)

        }   else {
            throw new GameException("ERROR : UNEXPECTED MESSAGE TYPE");
        }

        // now that the game is created, set the config values (original values)
        battleShipGame.getGame().setGameShips((byte) battleShipGame.getGame().getGameInfo().nships1, (byte) battleShipGame.getGame().getGameInfo().nships2, (byte) battleShipGame.getGame().getGameInfo().nships3, (byte) battleShipGame.getGame().getGameInfo().nships4, (byte) battleShipGame.getGame().getGameInfo().nships5);


        battleShipGame.getBsComUtils().write_OK(randomPlayerId, randomGameId); // Send OK message with random player and game ID
        System.out.println("Server : Game " + randomGameId + " , and player ID " + randomPlayerId + ", sent to the client with name " + battleShipGame.getGame().getPlayer1Name());
        battleShipGame.getGame().setPlayer2ID(-1); // bot player
        battleShipGame.getGame().setPlayer2AI(true); // change if player 2 is not a bot

        /*
        TODO : LOGIC FOR WAITING THE OTHER PLAYER (NOW ONLY ONE PLAYER)
        NO PLAYER 2 IMPLEMENTED YET, ONLY PLAYER 1 SO NO PLAYER 2 WAITING (BOT LOGIC)

        if ( battleShipGame.getGame().getPlayer2AI() ){
            // join from player 2
        }

         */

        battleShipGame.getGame().setGameState(CurrentState.SETUP); // Set game state to SETUP
        battleShipGame.randomBoard2(
                battleShipGame.getGame().getGameInfo().nships1,
                battleShipGame.getGame().getGameInfo().nships2,
                battleShipGame.getGame().getGameInfo().nships3,
                battleShipGame.getGame().getGameInfo().nships4,
                battleShipGame.getGame().getGameInfo().nships5); // Random board for player 2 (bot)

        // notify the player that the game is ready
        if (!battleShipGame.notifyStatus(randomPlayerId)) {
            throw new GameException("Error: notifyStatus failed");
        }

        System.out.println("Server : notify status sent");
        System.out.println("Server : Game settings initialized correctly");

    }

    /**
     * Creates a game with prefixed values
     * @param playerID id of the only player
     * @param gameID id of the game
     */
    public void initGameInfoFIXEDVALUES(int playerID, int gameID) {
        this.battleShipGame.setGame( new Game(gameID) );
        battleShipGame.getGame().setPlayer1ID(playerID);
    }

    /**
     * Creates a game with variable values set by the player
     * @param playerID id of the player
     * @param gameID id of the game
     * @param W width of the board
     * @param H height of the board
     * @param T1 number of ships of type 1
     * @param T2 number of ships of type 2
     * @param T3 number of ships of type 3
     * @param T4 number of ships of type 4
     * @param T5 number of ships of type 5
     * @param IA if the player is a bot
     * @return 1 if the game is possible, -1 if not
     */
    public int initGameInfoVARIABLEVALUES(int playerID, int gameID, int W, int H, int T1, int T2, int T3, int T4, int T5, int IA) {
        this.battleShipGame.setGame(new Game(gameID, W, H, T1, T2, T3, T4, T5, IA));
        battleShipGame.getGame().setPlayer1ID(playerID);
        return battleShipGame.getGame().getGameInfo().isBoardPosible();
    }

    /**
     * set the player 2 id
     * @param playerID id of the player 2 (bot), should be -1
     */
    public void setPlayer2ID(int playerID) {
        battleShipGame.getGame().setPlayer2ID(playerID);
    }


    /**
     * Receives the addvessel message and adds the vessel to the board.
     * @throws GameException error from the game flux
     * @throws IOException error from the communication
     */
    public void setup() throws GameException, IOException {

        while (battleShipGame.getGame().getGameState() == CurrentState.SETUP) {

            MessageAbstract message = battleShipGame.getBsComUtils().read(); // Read message

            // if message is GameConfig, send the config from the game
            if (message instanceof MessageGetConfig) {
                System.out.println("Server : getConfig received (in setup)");
                sendGameConfig();

                // if message is GameStatus, send the status from the game
            } else if (message instanceof MessageGetStatus) {
                System.out.println("Server : MessageGetStatus received (in setup)");
                this.battleShipGame.notifyStatus(((MessageGetStatus) message).getPlayerId());

                // if message is ADDV, add the vessel to the board
            } else if (message instanceof MessageAddVessel) {
                System.out.println("Server : AddVessel received");

                MessageAddVessel messageAddvessel = (MessageAddVessel) message;
                handleAddVessel(messageAddvessel);

                // if message is LEAVE, end the game
            } else if (message instanceof MessageLeave) {

                MessageLeave messageLeave = (MessageLeave) message;
                handleLeave(messageLeave);

                // shouldn't be reached (no possible error with OK message)
            } else if (message instanceof MessageError) { // Error message
                throw new GameException("Error: Received ERROR message");
                // shouldn't be reached (no possible error)
            } else {
                throw new GameException("Error: Unexpected message type");
            }
        }
    }


    /**
     * Manages the shots fired by the players and the bot.
     * @throws GameException error from the game flux
     * @throws IOException error from the communication
     */
    public void playing() throws GameException, IOException {

        System.out.println("Server : --------------- PLAYING ------------------");
        battleShipGame.printGame(); // see how the game is going

        while (battleShipGame.getGame().getGameState() == CurrentState.PLAYING) {

            MessageAbstract message = battleShipGame.getBsComUtils().read(); // Read message

            System.out.println("Server : PLAYING");
            battleShipGame.printGame(); // see how the game is going

            // if message is GameConfig, send the config from the game
            if (message instanceof MessageGetStatus) {
                System.out.println("Server : MessageGetStatus received (in playing)");
                this.battleShipGame.notifyStatus(((MessageGetStatus) message).getPlayerId());
                continue;

            } else if (message instanceof MessageGetConfig) {
                System.out.println("Server : getConfig received (in playing)");
                sendGameConfig();
                continue;

            } else if (message instanceof MessageLeave) {
                // LEAVE message
                handleLeave((MessageLeave) message);
                return;

            } else if (message instanceof MessageShot) {

                System.out.println("Server : Shot message received");
                MessageShot messageShot = (MessageShot) message;

                if (messageShot.getPlayerId() != battleShipGame.getActivePlayer()){
                    System.out.println("Server : Not the turn of the player");
                    battleShipGame.getBsComUtils().write_ERROR(ErrorCode.INVALID_PLAYER_ID.getErrorId(), ErrorCode.INVALID_PLAYER_ID.getDescription().length(), ErrorCode.INVALID_PLAYER_ID.getDescription());
                    continue;
                }


                switch (battleShipGame.shot(messageShot.getPlayerId(), messageShot.getR(), messageShot.getC())) {

                    case 0: //FAIL

                        System.out.println("Server : PLAYER FAIL ! from player " + messageShot.getPlayerId() + " at (" + messageShot.getR() + "," + messageShot.getC() +")" );
                        battleShipGame.getBsComUtils().write_FAIL(); // Send FAIL message

                        // Change the turn
                        battleShipGame.getGame().getGameInfo().tornJugador1 = battleShipGame.getGame().getGameInfo().tornJugador1 == 1 ? (byte) 0 : (byte) 1;
                        battleShipGame.getGame().getGameInfo().tornJugador2 = battleShipGame.getGame().getGameInfo().tornJugador2 == 1 ? (byte) 0 : (byte) 1;

                        break;
                    case 1: //HIT
                        System.out.println("Server : PLAYER Vessel HIT ! from player " + messageShot.getPlayerId() + " at (" + messageShot.getR() + "," + messageShot.getC() +")");
                        battleShipGame.getBsComUtils().write_HIT(false);
                        break;

                    case 2: //SUNK
                        System.out.println("Server : PLAYER Vessel SUNK ! from player " + messageShot.getPlayerId() + " at (" + messageShot.getR() + "," + messageShot.getC() +") ");
                        battleShipGame.getBsComUtils().write_HIT(true);

                        int winner = battleShipGame.getWinPlayer();
                        if (winner != -999) { //No vessels remaining
                            this.battleShipGame.endGame();
                        }
                        break;

                    case -1://ERROR
                        battleShipGame.getBsComUtils().write_ERROR(battleShipGame.getErrorCode().getErrorId(), battleShipGame.getErrorCode().getDescription().length(), battleShipGame.getErrorCode().getDescription());
                        continue; // retry shot
                }
                // Notify the player that the game is ready with gamestatus message
                this.battleShipGame.notifyStatus(messageShot.getPlayerId());

            } else {
                System.out.println("Server : Unexpected message type in playing");
                System.out.println("Server : recieved code : " + message.getMessageType());
            }

            if (battleShipGame.getGame().getPlayer2AI() && battleShipGame.getActivePlayer() == -1) { // playing against IA

                int resultShotIA = battleShipGame.shot(battleShipGame.getGame().getPlayer2ID());

                if (resultShotIA == -1) {
                    System.out.println("Server : bot didn't shot");
                } else if (resultShotIA == 2) {
                    System.out.println("Server : bot won, ending battleShipGame.getGame()...");
                    this.battleShipGame.endGame();
                    this.battleShipGame.notifyStatus(battleShipGame.getPlayerId(0));
                }

            } else {
                System.out.println(battleShipGame.getGame().getPlayer2AI());
                System.out.println("Server : Player 2 is not a bot ERROR");
            }

        }
    }




    /**
     * Ends the game, prints the info and closes the connection.
     * @throws GameException error from the game flux
     * @throws IOException error from the communication
     */
    public void finished() throws GameException, IOException {

        System.out.println("-----------------GAME OF PLAYER "+ battleShipGame.getGame().getPlayer1ID() + " ENDED --------");
        battleShipGame.printGame();
        handleLeave((MessageLeave) battleShipGame.getBsComUtils().read());

    }

    /**
     * send the game config to the player (always the same, as P2 is a bot)
     * The ships to add are not sent, only the original game settings
     * @throws IOException error from the communication
     */
    public void sendGameConfig() throws IOException {

        System.out.println("Server : Sending game config");
        System.out.println("Server : W = " + battleShipGame.getGame().getGameInfo().W);
        System.out.println("Server : H = " + battleShipGame.getGame().getGameInfo().H);

        battleShipGame.getBsComUtils().write_GAMECONFIG(
                battleShipGame.getGame().getGameInfo().W,
                battleShipGame.getGame().getGameInfo().H,
                battleShipGame.getGame().getShips1(),
                battleShipGame.getGame().getShips2(),
                battleShipGame.getGame().getShips3(),
                battleShipGame.getGame().getShips4(),
                battleShipGame.getGame().getShips5()); // Send GAMECONFIG message with the original game settings ( not the remaining ships to add )

        System.out.println("Server : Game config sent");
    }



    /**
     * Close the connection with the client
     */
    public void closeConnection(){
        try {
            System.out.println("Server (CLOSE CONNECTION) : Closing connection");
            battleShipGame.getBsComUtils().closeSocket();
            System.out.println("Server (CLOSE CONNECTION) : Connection closed");
        } catch (IOException e) {
            System.out.println("Server (CLOSE CONNECTION) : Error closing connection " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Check if the name is invalid
     * @param name the name to check
     * @return true if the name is invalid, false otherwise
     */
    private boolean isNameInvalid(String name){
        return name == null ||
                name.isEmpty() ||
                name.length() > 50;
    }


    /**
     * This method handles all the addvessels messages received
     * @param messageAddvessel the message received with the info of the vessel to add
     * @throws IOException if there is an error writing to the socket
     */
    private void handleAddVessel(MessageAddVessel messageAddvessel) throws IOException{

        boolean result = this.battleShipGame.addVessel(messageAddvessel.getPlayerId(),
                messageAddvessel.getRi(),
                messageAddvessel.getCi(),
                messageAddvessel.getRf(),
                messageAddvessel.getCf(),
                messageAddvessel.getType());

        // something went wrong, sending error message, saved in errorCode in addvessel
        if (!result) {
            System.out.println("Server : Vessel not added");
            battleShipGame.getBsComUtils().write_ERROR(battleShipGame.getErrorCode().getErrorId(), battleShipGame.getErrorCode().getDescription().length(), battleShipGame.getErrorCode().getDescription());
            System.out.println("Server : Error sent");


        } else { // everything went well, sending OK

            // print the board after adding the vessel
            if (messageAddvessel.getPlayerId() == battleShipGame.getGame().getPlayer1ID())
                this.battleShipGame.getGame().getGameInfo().printBoard1();
            else
                this.battleShipGame.getGame().getGameInfo().printBoard2();

            // send OK to keep going
            System.out.println("Server : Sending OK");
            battleShipGame.getBsComUtils().write_OK(
                    messageAddvessel.getPlayerId(),
                    messageAddvessel.getGameId()); // Send OK message

            // if all vessels are added, change game state to PLAYING
            if (battleShipGame.getGame().getGameInfo().getNshipsRemainingTotal() == 0) {
                System.out.println("Server : All vessels added, changing game state to PLAYING");
                System.out.println("Server : Sending Game State to PLAYING");
                battleShipGame.getGame().setGameState(CurrentState.PLAYING); // Set game state to PLAYING
                battleShipGame.notifyStatus(battleShipGame.getGame().getPlayer1ID());
                battleShipGame.getGame().getGameInfo().tornJugador1 = 1;
                battleShipGame.getGame().getGameInfo().tornJugador2 = 0;
            }
            System.out.println("Server : Vessel added and OK sent");
        }
    }

    /**
     * This method handles all the leave messages recieved
     * It sends the ok of the leave recieved and ends the game
     * @param messageLeave the message received with the info of the leave
     * @throws IOException if there is an error writing to the socket
     * @throws GameException if there is an error in the game
     */
    private void handleLeave(MessageLeave messageLeave) throws IOException, GameException {

        System.out.println("Server : Leave message received, ending game");

        if (messageLeave.getPlayerId() != battleShipGame.getGame().getPlayer1ID() ) {
            System.out.println("Server (HANDLE LEAVE): Invalid player ID");
            battleShipGame.getBsComUtils().write_ERROR(ErrorCode.INVALID_PLAYER_ID.getErrorId(), ErrorCode.INVALID_PLAYER_ID.getDescription().length(), ErrorCode.INVALID_PLAYER_ID.getDescription());
        }

        if (messageLeave.getGameId() != battleShipGame.getGame().getGameID()){
            System.out.println("Server (HANDLE LEAVE): Invalid game ID");
            battleShipGame.getBsComUtils().write_ERROR(ErrorCode.INVALID_GAME_ID.getErrorId(), ErrorCode.INVALID_GAME_ID.getDescription().length(), ErrorCode.INVALID_GAME_ID.getDescription());
        } else {
            battleShipGame.getBsComUtils().write_OK(
                    messageLeave.getPlayerId(),
                    messageLeave.getGameId()); // Send OK message
        }
        throw new GameException("Leave message received, ending game");
    }


}