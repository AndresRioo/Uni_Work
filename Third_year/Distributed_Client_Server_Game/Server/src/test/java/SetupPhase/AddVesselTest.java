package SetupPhase;

import org.junit.Test;
import p1.server.GameHandler;
import utils.BattleshipComUtils;
import utils.CurrentState;
import utils.Message.MessageAddVessel;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the AddVessel message when the vessel is added correctly
 */
public class AddVesselTest {


    /**
     * Test to check the AddVessel message when the vessel is added correctly
     */
    @Test
    public void message_ADDVESSEL_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte type = 2; // 4 cells
            byte ri = 1;
            byte ci = 1;
            byte rf = 4;
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

            assert checkAddVessel;

            System.out.println("Test AddVessel passed");

        } catch (IOException e) {
            System.out.println("FAILED : Test AddVessel ");
            e.printStackTrace();
        }
    }
}
