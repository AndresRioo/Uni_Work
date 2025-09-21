package SetupPhase;

import org.junit.Test;
import p1.server.GameHandler;
import utils.BattleshipComUtils;
import utils.CurrentState;
import utils.ErrorCode;
import utils.Message.MessageAddVessel;
import utils.Message.MessageShot;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the errors in the Shot message
 */
public class ShotErrorsTest {

    private GameHandler gameHandler;

    private int playerId = 55555;
    private int gameId = 55555;




    private void createGamehandler(){

        File file = new File("test");
        try {

            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(playerId, gameId);
            gameHandler.getGame().setGameState(CurrentState.SETUP);
            gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);
            gameHandler.setPlayer2ID(-1);

            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(-1, 1, 1, 5, 1, 1);
            assert checkAddVessel;

            gameHandler.getGame().setGameState(CurrentState.PLAYING);

            gameHandler.getGame().setOriginalBoard2( gameHandler.getBattleShipGame().getGame().getGameInfo().boardPlayer2 );
            gameHandler.getGame().setOriginalBoard1( gameHandler.getBattleShipGame().getGame().getGameInfo().boardPlayer1 );

            this.gameHandler = gameHandler;


        } catch (Exception e) {
            System.out.println("FAILED : createGamehandler in shot error test");
            e.printStackTrace();
        }
    }


    /**
     *  Send a shot message with an invalid coordinate (10000, 1000)
     */
    @Test
    public void message_Shot_ERROR_COORDINATE(){

        createGamehandler();

        int checkShot = gameHandler.getBattleShipGame().shot(playerId, 10000, 1000);

        assert gameHandler.getErrorCode() == ErrorCode.INVALID_COORDINATE;
        assert checkShot == -1; // shot a different row and column as sent

        System.out.println("Invalid shot passed");

    }

    /**
     *  Send a shot message with an invalid player id (4)
     */
    @Test
    public void message_Shot_ERROR_PLAYER_ID(){

        createGamehandler();

        int checkShot = gameHandler.getBattleShipGame().shot(4, 2, 2);

        assert gameHandler.getErrorCode() == ErrorCode.INVALID_PLAYER_ID;
        assert checkShot == -1; // shot a different row and column as sent

        System.out.println("Invalid player id passed");

    }

    /**
     *  Send a shot message with an invalid type
     */
    @Test
    public void message_Shot_ERROR_TYPE(){

        createGamehandler();
        System.out.println("-------------- ERROR TYPE TEST ----------");

        boolean result = gameHandler.getBattleShipGame().changeGameBoard( (byte) 1,(byte) 2,(byte) 3,(byte)2,(byte)7, false); // invalid type

        System.out.println("result --> " + result);
        System.out.println("Board with invalide type");
        gameHandler.getBattleShipGame().getGame().getGameInfo().printBoard2();

        int checkShot = gameHandler.getBattleShipGame().shot(playerId, 1, 1);

        assert gameHandler.getErrorCode() == ErrorCode.TYPE_UNAVAILABLE;
        assert checkShot == -1; // shot a different row and column as sent

        System.out.println("Invalid type passed");

    }


}
