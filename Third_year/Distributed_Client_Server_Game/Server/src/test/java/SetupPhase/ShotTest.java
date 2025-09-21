package SetupPhase;

import org.junit.Test;
import p1.server.GameHandler;
import utils.BattleshipComUtils;
import utils.CurrentState;
import utils.Message.MessageAddVessel;
import utils.Message.MessageShot;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class ShotTest {

    private GameHandler gameHandler;

    private int playerId = 55555;
    private int botId = -1;
    private int gameId = 55555;




    private void createGamehandler(){

        File file = new File("test");
        try {

            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoVARIABLEVALUES(playerId, gameId,3,3,0,0,0,0,1,1);
            gameHandler.getGame().setGameState(CurrentState.SETUP);
            gameHandler.getGame().getGameInfo().setWandH((byte) 3, (byte) 3);
            gameHandler.setPlayer2ID(botId);

            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(botId, 1, 1, 2, 1, 5);
            assert checkAddVessel;

            gameHandler.getGame().setOriginalBoard2( gameHandler.getBattleShipGame().getGame().getGameInfo().boardPlayer2 );

            gameHandler.getGame().setGameState(CurrentState.PLAYING);

            this.gameHandler = gameHandler;


        } catch (Exception e) {
            System.out.println("FAILED : createGamehandler in shot error test");
            e.printStackTrace();
        }
    }


    /**
     *  Send a FAIL shot message with everythign correct
     */
    @Test
    public void message_shot_test_fail() {

        createGamehandler();

        System.out.println("Before shot");
        gameHandler.getGame().getGameInfo().printBoard2();
        System.out.println("-------------------");

        byte rowAsAclient = 2;
        byte colAsAclient = 2; // 1 indexed

        rowAsAclient--; // 0 indexed
        colAsAclient--; // 0 indexed

        int resultShot = gameHandler.getBattleShipGame().shot(playerId, rowAsAclient, colAsAclient);

        System.out.println("After shot in (2,2) ");
        gameHandler.getGame().getGameInfo().printBoard2();

        assert resultShot == 0; // shot water (FAIL)


    }

    /**
     *  Send a HIT shot message with everythign correct
     */
    @Test
    public void message_shot_test_hit() {

        createGamehandler();

        System.out.println("Before shot");
        gameHandler.getGame().getGameInfo().printBoard2();
        System.out.println("-------------------");

        byte rowAsAclient = 1;
        byte colAsAclient = 1; // 1 indexed

        rowAsAclient--; // 0 indexed
        colAsAclient--; // 0 indexed

        int resultShot = gameHandler.getBattleShipGame().shot(playerId, rowAsAclient, colAsAclient);

        System.out.println("After shot in (1,1) ");
        gameHandler.getGame().getGameInfo().printBoard2();

        assert resultShot == 1; // shot hit (HIT)
    }


    /**
     *  Send a HIT shot message with everything correct
     *  Then send a HIT in the other cell, to sink the ship
     */
    @Test
    public void message_shot_test_sunk() {

        createGamehandler();

        System.out.println("Before shot");
        gameHandler.getGame().getGameInfo().printBoard2();
        System.out.println("-------------------");

        byte rowAsAclient = 1;
        byte colAsAclient = 1; // 1 indexed

        rowAsAclient--; // 0 indexed
        colAsAclient--; // 0 indexed

        int resultShot = gameHandler.getBattleShipGame().shot(playerId, rowAsAclient, colAsAclient);

        System.out.println("After shot in (1,1) HIT ");
        gameHandler.getGame().getGameInfo().printBoard2();

        assert resultShot == 1; // shot hit (HIT)


        rowAsAclient = 2;
        colAsAclient = 1; // 1 indexed

        rowAsAclient--; // 0 indexed
        colAsAclient--; // 0 indexed

        resultShot = gameHandler.getBattleShipGame().shot(playerId, rowAsAclient, colAsAclient);

        assert resultShot == 2; // shot hit (SUNK)

        System.out.println("After shot in (2,1) ");

        gameHandler.getGame().getGameInfo().printBoard2();

        System.out.println("----------------------");
        System.out.println("Original board");

        gameHandler.getGame().printOriginalBoard2();



    }



}
