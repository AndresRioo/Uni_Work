package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageAddVessel;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the AddVessel message
 */
public class BSUtilsTestAddVessel {

    @Test
    public void message_ADDVESSEL_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte type = 2;
            byte ri = 1;
            byte ci = 1;
            byte rf = 2;
            byte cf = 2;


            // Send data
            bsComUtils.write_ADDVESSEL(playerId,gameId,type,ri,ci,rf,cf);

            // Read data
            MessageAddVessel message = (MessageAddVessel) bsComUtils.read();


            // Check data consistency with asserts
            assert message.getMessageType() == 7 ; // code 7 AddVessel
            assert message.getPlayerId() == playerId;
            assert message.getGameId() == gameId;
            assert message.getType() == type;
            assert message.getRi() == ri;
            assert message.getCi() == ci;
            assert message.getRf() == rf;
            assert message.getCf() == cf;



            System.out.println("Test AddVessel passed");

        } catch (IOException e) {
            System.out.println("FAILED : Test AddVessel ");
            e.printStackTrace();
        }
    }
}
