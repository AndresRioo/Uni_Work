package SetupPhase;

import org.junit.Test;
import p1.server.GameHandler;
import utils.BattleshipComUtils;
import utils.CurrentState;
import utils.GameException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import static org.junit.Assert.assertTrue;

/**
 * Test class to check the randomBoard2 method (randomly place ships on the board for the bot)
 */
public class RandomBoard2Test {

    private GameHandler gameHandler;

    private int playerId = 55555;
    private int bot = -1;
    private int gameId = 55555;

    private void createGamehandler(){

        File file = new File("test");
        try {

            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(playerId, gameId);
            gameHandler.getGame().setGameState(CurrentState.SETUP);
            gameHandler.getGame().getGameInfo().setWandH((byte) 3, (byte) 3);
            gameHandler.setPlayer2ID(-1);

            this.gameHandler = gameHandler;


        } catch (Exception e) {
            System.out.println("FAILED : createGamehandler in shot error test");
            e.printStackTrace();
        }
    }

    /**
     * Test de randomBoard2 con 2 barcos de cada en un tablero de 10x10
     * @throws IOException
     */
    @Test
    public void randomBoard2() throws IOException {

        createGamehandler();

        //Inicia board2 automaticament
        gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);

        try {
            gameHandler.getBattleShipGame().randomBoard2(2,2,2,2,2);
        } catch (GameException e) {
            System.out.println(e.getMessage());
            assert false; // No deberia lanzar excepcion
        }
        assert true;
        gameHandler.getGame().getGameInfo().printBoard2();
    }

    /**
     * Test de randomBoard2 con 5 barcos de longitud 5 en un tablero de 5x5
     * @throws IOException
     */
    @Test
    public void randomBoard2SpecificType() throws IOException {

        createGamehandler();

        //Inicia board2 automaticament (5*5)
        gameHandler.getGame().getGameInfo().setWandH((byte) 5, (byte) 5);


        try {
            gameHandler.getBattleShipGame().randomBoard2(5,0,0,0,0); // 5 ships of len 5
            assert true;
        } catch (GameException e) {
            assert false;
            System.out.println(e.getMessage());
        }


        gameHandler.getGame().getGameInfo().printBoard2();
    }

    /**
     * Test de randomBoard2 con 5 barcos de longitud 3 en un tablero de 10x3
     * @throws IOException
     */
    @Test
    public void randomBoard2SpecificType2() throws IOException {

        createGamehandler();

        gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 3);


        try {
            gameHandler.getBattleShipGame().randomBoard2(0,0,5,0,0); // 5 ships of len 3
            assert true;
        } catch (GameException e) {
            assert false;
            System.out.println(e.getMessage());
        }

        gameHandler.getGame().getGameInfo().printBoard2();
    }

    /**
     * Test de randomBoard2 con 2 barcos de longitud 2 en un tablero de 3x3
     * @throws IOException
     */
    @Test
    public void randomBoard2SpecificType3() throws IOException {

        createGamehandler();


        gameHandler.getGame().getGameInfo().setWandH((byte) 3, (byte) 3);


        try {
            gameHandler.getBattleShipGame().randomBoard2(0,0,0,0,2); // 2 ships of len 2
            assert true;
        } catch (GameException e) {
            assert false;
            System.out.println(e.getMessage());
        }

        gameHandler.getGame().getGameInfo().printBoard2();
    }

    /**
     * Test de randomBoard2 with the max number of ships (25 of each)
     * @throws IOException
     */
    @Test
    public void randomBoard2_HUGE() throws IOException {

        createGamehandler();


        gameHandler.getGame().getGameInfo().setWandH((byte) 125, (byte) 6);


        try {
            gameHandler.getBattleShipGame().randomBoard2(25,25,25,25,25); // 2 ships of len 2
            assert true;
        } catch (GameException e) {
            assert false;
            System.out.println(e.getMessage());
        }

        gameHandler.getGame().getGameInfo().printBoard2();
    }


    /**
     * Test de randomBoard2 with more than the max number of ships (26)
     * @throws IOException
     */
    @Test
    public void randomBoard2_too_HUGE() throws IOException {

        createGamehandler();


        gameHandler.getGame().getGameInfo().setWandH((byte) 125, (byte) 6);


        try {
            gameHandler.getBattleShipGame().randomBoard2(26,0,0,0,0); // 2 ships of len 2
            assert false;
        } catch (GameException e) {
            assert true;
            System.out.println(e.getMessage());
        }

        gameHandler.getGame().getGameInfo().printBoard2();
    }

    /**
     * Test de error en randomBoard2, no se puede colocar los barcos en el tablero porque no hay espacio suficiente (no deberia pasar ya que el tamaño es predefinido)
     * @throws IOException
     */
    @Test
    public void randomBoard2ERROR() throws IOException {

        createGamehandler();

        gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);


        //Inicia board2 automaticament
        try {
            gameHandler.getBattleShipGame().randomBoard2(400,400,400,400,400);
            assert false;
        } catch (GameException e) {
            assert true;
            System.out.println(e.getMessage());
        }

        gameHandler.getGame().getGameInfo().printBoard2();
    }


}
