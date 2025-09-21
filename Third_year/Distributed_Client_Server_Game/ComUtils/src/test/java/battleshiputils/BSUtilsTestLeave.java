package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageLeave;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the LEAVE message
 */
public class BSUtilsTestLeave {

    @Test
    public void message_LEAVE_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            int playerId = 55555;
            int gameId = 55555;


            // Send data
            bsComUtils.write_LEAVE( playerId, gameId );

            // Read data
            MessageLeave message = (MessageLeave) bsComUtils.read();


            // Check data consistency with asserts
            assert message.getMessageType() == 13 ; // code 13 Leave
            assert message.getPlayerId() == playerId;
            assert message.getGameId() == gameId;


            System.out.println("Test LEAVE passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Leave ");
            e.printStackTrace();
        }
    }


}
