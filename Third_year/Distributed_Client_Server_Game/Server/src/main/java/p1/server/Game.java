package p1.server;

import utils.GameInfo;
import utils.CurrentState;

/**
 * Class that represents a game for the server (Contains the game state, the players, the boards, etc)
 */
public class Game {

    /**
     * GameInfo object with the information of the game.
     */
    private utils.GameInfo gameInfo;

    /**
     * ID of the game.
     */
    private int gameID;

    /**
     * ID of the player 1.
     */
    private int player1ID;

    /**
     * Name of the player 1.
     */
    private String player1Name;

    /**
     * ID of the player 2.
     */
    private int player2ID;

    /**
     * Name of the player 2.
     */
    private String player2Name;

    /**
     * Boolean that indicates if player 2 is an AI.
     */
    private boolean player2AI;

    /**
     * Original board of player 1, with the ships before shooting.
     */
    private byte[] originalBoard1;

    /**
     * Original board of player 2, with the ships before shooting.
     */
    private byte[] originalBoard2;

    /**
     * Number of ships of each type for the game (the original value, not the current value)
     */
    private byte ships1;

    /**
     * Number of ships of each type for the game (the original value, not the current value)
     */
    private byte ships2;

    /**
     * Number of ships of each type for the game (the original value, not the current value)
     */
    private byte ships3;

    /**
     * Number of ships of each type for the game (the original value, not the current value)
     */
    private byte ships4;

    /**
     * Number of ships of each type for the game (the original value, not the current value)
     */
    private byte ships5;


    /**
     * Constructor of the class with prefixed values.
     * @param GameID ID of the game.
     */
    public Game(int GameID ){
        this.gameID = GameID;
        this.gameInfo = new GameInfo();
        this.gameInfo.setGameState(CurrentState.WAITING_PLAYERS);
    }

    /**
     * Constructor of the class with custom values , selected by the user.
     * @param gameID ID of the game.
     * @param W Width of the board.
     * @param H Height of the board.
     * @param T1 Number of ships of type 1.
     * @param T2 Number of ships of type 2.
     * @param T3 Number of ships of type 3.
     * @param T4 Number of ships of type 4.
     * @param T5 Number of ships of type 5.
     * @param IA Number of AI players.
     */
    public Game(int gameID, int W, int H, int T1, int T2, int T3, int T4, int T5, int IA){
        this.gameID = gameID;
        this.gameInfo = new GameInfo(W,H,T1,T2,T3,T4,T5,IA);
        this.gameInfo.setGameState(CurrentState.WAITING_PLAYERS);

    }

    public void setPlayer1ID(int Player1ID){
        this.player1ID = Player1ID;
    }

    public void setPlayer2ID(int Player2ID){
        this.player2ID = Player2ID;
    }

    public void setPlayer1Name(String Player1Name){
        this.player1Name = Player1Name;
    }

    public void setPlayer2Name(String Player2Name){
        this.player2Name = Player2Name;
    }

    public void setPlayer2AI(boolean Player2AI){
        this.player2AI = Player2AI;
    }

    public boolean getPlayer2AI(){
        return this.player2AI;
    }

    public String getPlayer1Name(){
        return this.player1Name;
    }

    public String getPlayer2Name(){
        return this.player2Name;
    }

    public int getPlayer1ID(){
        return this.player1ID;
    }

    public int getPlayer2ID(){
        return this.player2ID;
    }

    public void setGameState(byte gameState){
        this.gameInfo.setGameState(gameState);
    }
    public void setGameState(CurrentState gameState){
        this.gameInfo.setGameState(gameState);
    }

    public CurrentState getGameState(){
        return this.gameInfo.getGameState();
    }

    public utils.GameInfo getGameInfo(){
        return this.gameInfo;
    }

    public int getGameID(){
        return this.gameID;
    }

    /**
     * Copies the original board of player 1 (no reference).
     * @param board
     */
    public void setOriginalBoard1(byte[] board){
        this.originalBoard1 = board.clone();
    }

    /**
     * Copies the original board of player 2 (no reference).
     * @param board
     */
    public void setOriginalBoard2(byte[] board){
        this.originalBoard2 = board.clone();
    }

    public byte[] getOriginalBoard1(){
        return this.originalBoard1;
    }

    public byte[] getOriginalBoard2(){
        return this.originalBoard2;
    }

    /**
     * Sets the number of ships of each type for the game (the original value, not the current value)
     * @param ships1  Number of ships of type 1.
     * @param ships2 Number of ships of type 2.
     * @param ships3 Number of ships of type 3.
     * @param ships4 Number of ships of type 4.
     * @param ships5 Number of ships of type 5.
     */
    public void setGameShips(byte ships1, byte ships2, byte ships3, byte ships4, byte ships5){
        this.ships1 = ships1;
        this.ships2 = ships2;
        this.ships3 = ships3;
        this.ships4 = ships4;
        this.ships5 = ships5;
    }

    public byte getShips1(){
        return this.ships1;
    }

    public byte getShips2(){
        return this.ships2;
    }

    public byte getShips3(){
        return this.ships3;
    }

    public byte getShips4(){
        return this.ships4;
    }

    public byte getShips5(){
        return this.ships5;
    }

    /**
     * Prints the original board of player 1.
     */
    public void printOriginalBoard1(){
        for (int i = 0; i < gameInfo.H; i++) {
            for (int j = 0; j < gameInfo.W; j++) {
                System.out.print(originalBoard1[i * gameInfo.W + j] + " ");
            }
            System.out.println();
        }
    }

    /**
     * Prints the original board of player 2.
     */
    public void printOriginalBoard2(){
        for (int i = 0; i < gameInfo.H; i++) {
            for (int j = 0; j < gameInfo.W; j++) {
                System.out.print(originalBoard2[i * gameInfo.W + j] + " ");
            }
            System.out.println();
        }
    }
}
