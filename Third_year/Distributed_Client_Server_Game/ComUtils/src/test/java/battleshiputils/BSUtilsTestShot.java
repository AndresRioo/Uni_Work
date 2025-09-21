package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageShot;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the SHOT message
 */
public class BSUtilsTestShot {

    @Test
    public void message_SHOT_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;
            byte r = 1;
            byte c = 1;


            // Send data
            bsComUtils.write_SHOT( playerId, gameId, r, c );

            // Read data
            MessageShot message = (MessageShot) bsComUtils.read();


            // Check data consistency with asserts
            assert message.getMessageType() == 10 ; // code 10 Shot
            assert message.getPlayerId() == playerId;
            assert message.getGameId() == gameId;
            assert message.getR() == r;
            assert message.getC() == c;



            System.out.println("Test SHOT passed");

        } catch (IOException e) {
            System.out.println("FAILED : Test Shot ");
            e.printStackTrace();
        }
    }


}
