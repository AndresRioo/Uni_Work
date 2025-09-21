package SetupPhase;

import org.junit.Test;
import p1.server.GameHandler;
import utils.BattleshipComUtils;
import utils.CurrentState;
import utils.ErrorCode;
import utils.Message.MessageAddVessel;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the errors in the AddVessel message
 */
public class AddVesselErrorsTest {

    /**
     * Test to check the error message when the vessel is added with an invalid size
     */
    @Test
    public void message_ADDVESSEL_ERRORSIZE_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte type = 2; // 4 cells
            byte ri = 1;
            byte ci = 1;
            byte rf = 5; // Error ( 5 != 4 )
            byte cf = 1;


            // Send data
            bsComUtils.write_ADDVESSEL(playerId,gameId,type,ri,ci,rf,cf);

            // Read data
            MessageAddVessel message = (MessageAddVessel) bsComUtils.read();

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(message.getPlayerId(), message.getGameId());
            gameHandler.getGame().setGameState(CurrentState.SETUP);

            gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);

            int playerIdMsg = message.getPlayerId();
            int gameIdMsg = message.getGameId();
            byte typeMsg = message.getType();
            byte riMsg = message.getRi();
            byte ciMsg = message.getCi();
            byte rfMsg = message.getRf();
            byte cfMsg = message.getCf();

            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerIdMsg, riMsg, ciMsg, rfMsg, cfMsg, typeMsg);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 7 (INVALID_LENGTH)
            assert gameHandler.getErrorCode() == ErrorCode.INVALID_LENGTH;

            System.out.println("Test AddVessel passed (Didn't add the vessel because of the size error (type 2 with 5 cells) )");

        } catch (IOException e) {
            System.out.println("FAILED : Test AddVessel ");
            e.printStackTrace();
        }
    }


    /**
     * Test to check the error message when the vessel is added with an invalid type
     */
    @Test
    public void message_ADDVESSEL_TIPUSNODISPONIBLE_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte type = 6; // Error ( 6 != 1,2,3,4,5 )
            byte ri = 1;
            byte ci = 1;
            byte rf = 5;
            byte cf = 1;

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(playerId, gameId);
            gameHandler.getGame().setGameState(CurrentState.SETUP);
            gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);


            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 8 (TYPE_UNAVAILABLE)
            assert gameHandler.getErrorCode() == ErrorCode.TYPE_UNAVAILABLE;

            System.out.println("Test AddVessel passed (Didn't add the vessel because of the Type (6) )");

        } catch (IOException e) {
            System.out.println("FAILED : Test AddVessel ");
            e.printStackTrace();
        }
    }

    /**
     * Test to check the error message when the vessel is added with an invalid coordinate
     */
    @Test
    public void message_ADDVESSEL_COORDENADA_INCORRECTA_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte type = 1;
            byte ri = 1;
            byte ci = 1;
            byte rf = 5;
            byte cf = 2; // Error diagonal ship

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(playerId, gameId);
            gameHandler.getGame().setGameState(CurrentState.SETUP);
            gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);


            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 9 (INVALID_COORDINATE)
            assert gameHandler.getErrorCode() == ErrorCode.INVALID_COORDINATE;

            type = 1;
            ri = 0; // Error boards starts at 1
            ci = 1;
            rf = 4;
            cf = 1;

            checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 9 (INVALID_COORDINATE)
            assert gameHandler.getErrorCode() == ErrorCode.INVALID_COORDINATE;


            type = 1; // 5 cells
            ri = 9;
            ci = 1;
            rf = 13; // Error out of bounds ( 13 != 1,2,3,4,5,6,7,8,9,10 )
            cf = 1;

            checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 9 (INVALID_COORDINATE)
            assert gameHandler.getErrorCode() == ErrorCode.INVALID_COORDINATE;

            type = 1; // 5 cells
            ri = 1;
            ci = 3;
            rf = 5;
            cf = 3; // All fine

            checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            assert checkAddVessel;

            type = 1; // 5 cells
            ri = 3;
            ci = 1;
            rf = 3;
            cf = 5; // collision with the previous ship

            checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 9 (INVALID_COORDINATE)
            assert gameHandler.getErrorCode() == ErrorCode.INVALID_COORDINATE;

            System.out.println("Test AddVessel passed (Didn't add the vessel because of wrong coords )");

        } catch (IOException e) {
            System.out.println("FAILED : Test AddVessel ");
            e.printStackTrace();
        }
    }

    /**
     * Test to check the error message when the vessel is added with an invalid state
     */
    @Test
    public void message_ADDVESSEL_WRONGState_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte type = 6; // Error ( 6 != 1,2,3,4,5 )
            byte ri = 1;
            byte ci = 1;
            byte rf = 5;
            byte cf = 1;

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(playerId, gameId);
            gameHandler.getGame().getGameInfo().setWandH((byte) 10, (byte) 10);


            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, ri, ci, rf, cf, type);

            // the Server didn't add the vessel
            assert !checkAddVessel;

            // the Server sent an error message with code 10 (INVALID_STATE)
            assert gameHandler.getErrorCode() == ErrorCode.INVALID_STATE;

            System.out.println("Test AddVessel passed (Didn't add the vessel because of the wrong State (not SETUP) )");

        } catch (IOException e) {
            System.out.println("FAILED : Test AddVessel ");
            e.printStackTrace();
        }
    }
}
