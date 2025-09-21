package utils;

import java.util.Arrays;

/**
 * Class for server and client, to store the game information.
 */
public class GameInfo {

    /**
     * Enum for the different states of the game.
     */
    private CurrentState gameState;

    /**
     * Width of the board
     */
    public byte W;

    /**
     * Height of the board
     */
    public byte H;

    /**
     * Size of the board (W*H)
     */
    public int boardSize;

    /**
     * Board of player 1
     */
    public byte[] boardPlayer1;

    /**
     * Board of player 2
     */
    public byte[] boardPlayer2;

    /**
     * Flag to indicate if the player 1 is ready
     */
    public byte isJugador1Preparat;

    /**
     * Flag to indicate if the player 2 is ready
     */
    public byte isJugador2Preparat;

    /**
     * Number of pendent ships of type 1 to add
     */
    public byte nships1;
    /**
     * Number of pendent ships of type 2 to add
     */
    public byte nships2;
    /**
     * Number of pendent ships of type 3 to add
     */
    public byte nships3;
    /**
     * Number of pendent ships of type 4 to add
     */
    public byte nships4;

    /**
     * Number of pendent ships of type 5 to add
     */
    public byte nships5;

    /**
     * flag with the turn of the player 1
     * */
    public byte tornJugador1;

    /**
     * flag with the turn of the player 2
     * */
    public byte tornJugador2;

    /**
     * flag to indicate if the player 1 has won
     */
    public byte guanyaJugador1;

    /**
     * flag to indicate if the player 2 has won
     */
    public byte guanyaJugador2;

    /**
     * Instance of the ship 1 (starts at the ship number 1)
     */
    private byte instanceType1 = 1;

    /**
     * Instance of the ship 2 (starts at the ship number 1)
     */
    private byte instanceType2 = 1;

    /**
     * Instance of the ship 3 (starts at the ship number 1)
     */
    private byte instanceType3 = 1;

    /**
     * Instance of the ship 4 (starts at the ship number 1)
     */
    private byte instanceType4 = 1;

    /**
     * Instance of the ship 5 (starts at the ship number 1)
     */
    private byte instanceType5 = 1;

    /**
     * Type of the ship sent
     */
    private byte typeSent;

    /**
     * IA flag ( playing against the computer or not)
     */
    public byte IA;

    /**
     * Default constructor with default values (JOIN with 1 player will have this values)
     */
    public GameInfo() {

        System.out.println("Default board with W: 3 H: 3");

        this.gameState = CurrentState.WAITING_PLAYERS;

        this.setWandH((byte) 3, (byte) 3);

        this.isJugador1Preparat = 0;
        this.isJugador2Preparat = 0;

        this.nships1 = 0; // game with 1 ship (faster)
        this.nships2 = 0;
        this.nships3 = 0;
        this.nships4 = 0;
        this.nships5 = 1;

        this.tornJugador1 = 1;
        this.tornJugador2 = 0;

        this.guanyaJugador1 = 0;
        this.guanyaJugador2 = 0; // no one has won yet

        this.IA = 1;
    }

    /**
     * Constructor with no values of the game (used just to fill it with custom values later)
     * @param res doesn't matter, just to differentiate from the other constructor
     */
    public GameInfo(int res) {

        this.tornJugador1 = 1;
        this.tornJugador2 = 0;

        this.guanyaJugador1 = 0;
        this.guanyaJugador2 = 0;

        this.IA = 1;
    }

    /**
     * Constructor with custom values (CREATE games)
     * @param W width
     * @param H height
     * @param T1 number of ships of type 1
     * @param T2 number of ships of type 2
     * @param T3 number of ships of type 3
     * @param T4 number of ships of type 4
     * @param T5 number of ships of type 5
     * @param IA flag to indicate if the game is against the computer
     */
    public GameInfo(int W, int H, int T1, int T2, int T3, int T4, int T5, int IA){

        System.out.println("Custom board with W: " + W + " H: " + H + " T1: " + T1 + " T2: " + T2 + " T3: " + T3 + " T4: " + T4 + " T5: " + T5 + " IA: " + IA);

        this.gameState = CurrentState.WAITING_PLAYERS;

        this.W = (byte) W;
        this.H = (byte) H;
        this.boardSize = W * H;
        this.boardPlayer1 = new byte[boardSize];
        this.boardPlayer2 = new byte[boardSize];

        Arrays.fill(this.boardPlayer1, (byte) 0);
        Arrays.fill(this.boardPlayer2, (byte) 0);

        this.isJugador1Preparat = 0;
        this.isJugador2Preparat = 0;

        this.nships1 = (byte) T1;
        this.nships2 = (byte) T2;
        this.nships3 = (byte) T3;
        this.nships4 = (byte) T4;
        this.nships5 = (byte) T5;

        this.tornJugador1 = 1;
        this.tornJugador2 = 0;

        this.guanyaJugador1 = 0;
        this.guanyaJugador2 = 0;

        this.IA = (byte) IA;


    }

    /**
     * get the current state of the game
     * @return the current state of the game
     */
    public CurrentState getGameState() {
        return gameState;
    }

    /**
     * Copy the gamestatus without overriding W,H and other params
     * @param gameInfo gameinfo to copy
     */
    public void setNewGameInfo(GameInfo gameInfo){


        this.gameState = gameInfo.getGameState();
        this.boardSize = gameInfo.boardSize;
        this.boardPlayer1 = gameInfo.boardPlayer1;
        this.boardPlayer2 = gameInfo.boardPlayer2;

        switch (gameState){
            case WAITING_PLAYERS:
                break;
            case SETUP:
                this.isJugador1Preparat = gameInfo.isJugador1Preparat;
                this.isJugador2Preparat = gameInfo.isJugador2Preparat;
                this.nships1 = gameInfo.nships1;
                this.nships2 = gameInfo.nships2;
                this.nships3 = gameInfo.nships3;
                this.nships4 = gameInfo.nships4;
                this.nships5 = gameInfo.nships5;
                break;
            case PLAYING:
                this.tornJugador1 = gameInfo.tornJugador1;
                this.tornJugador2 = gameInfo.tornJugador2;
                break;
            case FINISHED:
                this.guanyaJugador1 = gameInfo.guanyaJugador1;
                this.guanyaJugador2 = gameInfo.guanyaJugador2;
                break;
        }


    }

    /**
     * get if the player 1 has won
     * @return 1 if the player 1 has won, 0 if not
     */
    public byte getGuanyaJugador1() {
        return guanyaJugador1;
    }

    /**
     * get if the player 2 has won
     * @return 1 if the player 2 has won, 0 if not
     */
    public byte getGuanyaJugador2() {
        return guanyaJugador2;
    }


    /**
     * Setter if the player 1 has won
     * @param GuanyaJugador1  1 if the player 1 has won, 0 if not
     */
    public void setGuanyaJugador1(byte GuanyaJugador1) {
        this.guanyaJugador1 = GuanyaJugador1;
    }

    /**
     * Setter if the player 2 has won
     * @param GuanyaJugador2  1 if the player 2 has won, 0 if not
     */
    public void setGuanyaJugador2(byte GuanyaJugador2) {
        this.guanyaJugador2 = GuanyaJugador2;
    }

    /**
     * Set W and H of the board (avoid size of the arrays to not be the same)
     * @param W width
     * @param H height
     */
    public void setWandH(byte W, byte H){
        this.W = W;
        this.H = H;
        this.boardSize = W * H;
        this.boardPlayer1 = new byte[boardSize];
        this.boardPlayer2 = new byte[boardSize];

        Arrays.fill(this.boardPlayer1, (byte) 0);
        Arrays.fill(this.boardPlayer2, (byte) 0);
    }

    /**
     * Set the current state of the game
     * @param gameState the current state of the game
     */
    public void setGameState(CurrentState gameState) {
        this.gameState = gameState;
    }

    /**
     * Set the current state of the game
     * @param gameState the current state of the game in byte
     */
    public void setGameState(byte gameState) {
        this.gameState = CurrentState.fromCode(gameState);
    }

    /**
     * Returns the info in function of the current state of the game
     * @param gameState the current state of the game
     * @return the info in function of the current state of the game
     * @throws Exception if the game state is invalid
     */
    public byte[] getInfo(CurrentState gameState) throws Exception {
        byte[] info;

        switch (gameState) {
            case WAITING_PLAYERS:
                return null;

            case SETUP:
                info = new byte[7];
                info[0] = isJugador1Preparat;
                info[1] = isJugador2Preparat;
                info[2] = nships1;
                info[3] = nships2;
                info[4] = nships3;
                info[5] = nships4;
                info[6] = nships5;
                return info;

            case PLAYING:
                info = new byte[2];
                info[0] = tornJugador1;
                info[1] = tornJugador2;
                return info;

            case FINISHED:
                info = new byte[2];
                info[0] = guanyaJugador1;
                info[1] = guanyaJugador2;
                return info;

            default:
                throw new IllegalArgumentException("Invalid game state");
        }
    }

    /**
     * Get the type of the ship sent
     * @return the type of the ship sent
     */
    public byte getTypeSent() {
        return typeSent;
    }

    /**
     * Set the type of the ship sent
     * @param typeSent the type of the ship sent
     */
    public void setTypeSent(byte typeSent) {
        this.typeSent = typeSent;
    }

    /**
     * Print the board of the player 1
     */
    public void printBoard1() {
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                System.out.print(boardPlayer1[i * W + j] + " ");
            }
            System.out.println();
        }
    }

    /**
     * Print the board of the player 2
     */
    public void printBoard2() {
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                System.out.print(boardPlayer2[i * W + j] + " ");
            }
            System.out.println();
        }
    }

    /**
     * Print the info in the terminal (debugging)
     * @param gameState the current state of the game
     */
    public void printInfo(CurrentState gameState){

        System.out.println("Estat actual de la partida : " + gameState);

        switch ( gameState ){
            case WAITING_PLAYERS:
                break;
            case SETUP:
                System.out.println("Esta el jugador 1 preparat ? (1 si, 0 no): " + isJugador1Preparat);
                System.out.println("Esta el jugador 2 preparat ? (1 si, 0 no): " + isJugador2Preparat);
                System.out.println("Vaixells del tipus " + ShipType.fromCode((byte)1) + " per afegir --> " + nships1 );
                System.out.println("Vaixells del tipus " + ShipType.fromCode((byte)2) + " per afegir --> " + nships2 );
                System.out.println("Vaixells del tipus " + ShipType.fromCode((byte)3) + " per afegir --> " + nships3 );
                System.out.println("Vaixells del tipus " + ShipType.fromCode((byte)4) + " per afegir --> " + nships4 );
                System.out.println("Vaixells del tipus " + ShipType.fromCode((byte)5) + " per afegir --> " + nships5 );
                break;
            case PLAYING:

                if (tornJugador1 == 1){
                    System.out.println("Torn del jugador 1 !");}
                else{
                    System.out.println("Torn del jugador 2 !");
                }

                break;
            case FINISHED:
                if (guanyaJugador1 == 1){
                    System.out.println("Ha guanyat el jugador 1 !");
                }else if (guanyaJugador2 == 1){
                    System.out.println("Ha guanyat el jugador 2 !");
                }else{
                    System.out.println("ERROR NO HA GUANYAT NINGU !");
                }
                break;
            default:
                System.out.println("Invalid game state");
                break;
        }
    }

    /**
     * transform the row and column to an index of the board
     * @param row row of the board
     * @param col column of the board
     * @return the index of the board
     */
    public int fromBoardToIndex(int row, int col){
        return row * W + col;
    }

    /**
     * transform the index of the board to the row and column
     * @param index index of the board
     * @param coords array to store the row and column
     */
    public void fromIndexToBoard(int index, int[] coords){
        coords[0] = index % W;
        coords[1] = index / W;
    }

    /**
     * set the value of the board of the player 1, with the row and column
     * @param row row of the board
     * @param col column of the board
     * @param value value to set
     */
    public void setBoard1Coords(int row, int col, byte value){
        boardPlayer1[fromBoardToIndex(row, col)] = value;
    }

    /**
     * set the value of the board of the player 2, with the row and column
     * @param row row of the board
     * @param col column of the board
     * @param value value to set
     */
    public void setBoard2Coords(int row, int col, byte value){
        boardPlayer2[fromBoardToIndex(row, col)] = value;
    }

    /**
     * get the value of the board of the player 1, with the row and column
     * @param row row of the board
     * @param col column of the board
     * @return the value of the board
     */
    public byte getBoard1Coords(int row, int col){
        return boardPlayer1[fromBoardToIndex(row, col)];
    }

    /**
     * get the value of the board of the player 2, with the row and column
     * @param row row of the board
     * @param col column of the board
     * @return the value of the board
     */
    public byte getBoard2Coords(int row, int col){
        return boardPlayer2[fromBoardToIndex(row, col)];
    }

    /**
     * Get the number of ships of a given type
     * @param type type of the ship
     * @return the number of ships of the given type
     */
    public byte getNShipsRemaining(int type){
        switch (type){
            case 1:
                return nships1;
            case 2:
                return nships2;
            case 3:
                return nships3;
            case 4:
                return nships4;
            case 5:
                return nships5;
            default:
                return -1;
        }
    }

    /**
     * Get the total number of ships remaining to add
     * @return the total number of ships remaining to add
     */
    public byte getNshipsRemainingTotal(){
        return (byte) (nships1 + nships2 + nships3 + nships4 + nships5);
    }

    /**
     * Decrease the number of ships of a given type
     * @param type type of the ship
     */
    public void decreaseNShips(int type){
        switch (type){
            case 1:
                nships1--;
                break;
            case 2:
                nships2--;
                break;
            case 3:
                nships3--;
                break;
            case 4:
                nships4--;
                break;
            case 5:
                nships5--;
                break;
            default:
                System.out.println("ERROR: INVALID TYPE (decreaseNships) --> " + type);
                break;
        }
    }

    /**
     * Check if there are any ships remaining
     * @return true if there are any ships remaining, false if not
     */
    public boolean isThereAnyShipRemaining(){
        return nships1 > 0 || nships2 > 0 || nships3 > 0 || nships4 > 0 || nships5 > 0;
    }

    /**
     * Get the instance of the ship and increase the instance, so the next ship will have a different instance
     * @param type type of the ship
     * @return the instance of the ship
     */
    public byte getInstanceAndIncrease(int type) {

        switch (type){
            case 1:
                byte temp = instanceType1;
                instanceType1++;
                return temp;
            case 2:
                byte temp2 = instanceType2;
                instanceType2++;
                return temp2;
            case 3:
                byte temp3 = instanceType3;
                instanceType3++;
                return temp3;
            case 4:
                byte temp4 = instanceType4;
                instanceType4++;
                return temp4;
            case 5:
                byte temp5 = instanceType5;
                instanceType5++;
                return temp5;
            default:
                return -1;
        }
    }

    /**
     * Check if the ship is sunk (all the instances of the ship are hit)
     * @param playerIndex player index
     * @param r row
     * @param c column
     * @param instance instance of the ship
     * @param type type of the ship
     * @return true if the ship is sunk, false if not
     */
    public boolean isLastInstanceOfShip(int playerIndex, int r, int c, int instance, int type){

        int index = fromBoardToIndex(r, c);

        for (int i = 0; i < boardPlayer2.length; i++) { // check the other board

            if (index == i) { // skip the current index of the ship
                continue;
            }

            // check if the ship is not sunk by checking
            // the instance is not elsewhere
            // the type is the same
            // the ship type is not 0
            if (playerIndex == 2 ) {
                if (boardPlayer2[i] / 10 == instance &&  // instance can be negative
                        Math.abs(boardPlayer2[i] % 10) == type &&  // type can't be negative!
                        boardPlayer2[i] % 10 != 0) {
                    return false;
                }
            } else {
                if (boardPlayer1[i] / 10 == instance &&
                        Math.abs(boardPlayer1[i] % 10) == type &&
                        boardPlayer1[i] % 10 != 0) {
                    return false;
                }
            }
        }
        return true; // if no other instance of the ship is found
    }


    /**
     * Check if player 1 has still ships
     * @return true if player 1 has still ships, false if not
     */
    private boolean isPlayer1StillAlive() {
        for (byte value : boardPlayer1) {
            if (value != 0 && value != 10 && value != 20 && value != 30) {
                System.out.println("PLAYER 1 HAS STILL SHIPS --> " + value);
                return true; // player 1 has still ships
            }
        }
        return false; // player 1 has no ships
    }

    /**
     * Check if player 2 has still ships
     * @return true if player 2 has still ships, false if not
     */
    private boolean isPlayer2StillAlive() {

        for (byte value : boardPlayer2) {
            if (value != 0 && value != 10 && value != 20 && value != 30) {
                System.out.println("PLAYER 2 HAS STILL SHIPS --> " + value);
                return true; // player 2 has still ships
            }
        }

        System.out.println("PLAYER 2 HAS NO SHIPS, END OF GAME" );
        return false; // player 2 has no ships
    }

    /**
     * Check if the player has won
     * @param playerIdx player index
     * @return true if the player has won, false if not
     */
    public boolean hasThePlayerWon(int playerIdx) {
        if (playerIdx == 1) { // check if player 1 has won
            return !isPlayer2StillAlive(); // if player 2 has no ships, player 1 has won
        } else if (playerIdx == 2) { // check if player 2 has won
            return !isPlayer1StillAlive(); // if player 1 has no ships, player 2 has won
        } else if (playerIdx == -1) {
            return false; // -1 equals the state is FINISHED
        } else {
            return false;
        }

    }

    /**
     * Check if the board is possible (naive approach)
     * @return 0 if the board is possible, -1 if not
     */
    public int isBoardPosible(){

        int caselles = W*H;
        int espaiVaixell = nships1*5 + nships2*4 + nships3*3 + nships4*3 + nships5*2;

        if (espaiVaixell > caselles){ // not enough space
            System.out.println("1. BOARD NOT POSSIBLE : NOT ENOUGH SPACE");
            return -1;
        }

        if (espaiVaixell == 0){ // no ships
            System.out.println("2. BOARD NOT POSSIBLE : NO SHIPS");
            return -1;
        }

        if (caselles < 4){ // the minimum board size is 2*2 (2*1 doesn't make sense)
            System.out.println("3. BOARD NOT POSSIBLE : BOARD TOO SMALL");
            return -1;
        }

        if (nships1 >= 1 && (W < 5 && H < 5)) { // ship 1 needs 5 spaces
            System.out.println("4. BOARD NOT POSSIBLE : SHIP 1 TOO BIG");
            return -1; // doesnt fit
        }

        if (nships2 >= 1 && (W < 4 && H < 4)) { // ship 2 needs 4 spaces
            System.out.println("5. BOARD NOT POSSIBLE : SHIP 2 TOO BIG");
            return -1; // doesnt fit
        }

        if (nships3 >= 1 && (W < 3 && H < 3)) { // ship 3 needs 3 spaces
            System.out.println("6. BOARD NOT POSSIBLE : SHIP 3 TOO BIG");
            return -1; // doesnt fit
        }

        if (nships4 >= 1 && (W < 3 && H < 3)) { // ship 4 needs 3 spaces
            System.out.println("7. BOARD NOT POSSIBLE : SHIP 4 TOO BIG");
            return -1; // doesnt fit
        }

        if (nships5 >= 1 && (W < 2 && H < 2)) { // ship 5 needs 2 spaces
            System.out.println("8. BOARD NOT POSSIBLE : SHIP 5 TOO BIG");
            return -1; // doesnt fit
        }

        // protocol limits the number of ships to 25
        if (nships1 > 25 || nships2 > 25 || nships3 > 25 || nships4 > 25 || nships5 > 25){
            System.out.println("9. BOARD NOT POSSIBLE : TOO MANY SHIPS");
            return -1; // too many ships
        }

        return 0; // board is possible
    }
}
