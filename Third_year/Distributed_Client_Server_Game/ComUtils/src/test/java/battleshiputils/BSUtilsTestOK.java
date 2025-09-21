package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;

import utils.Message.MessageAbstract;
import utils.Message.MessageOK;

import utils.Message.MessageAbstract;
import utils.Message.MessageOK;

/**
 * Test class to check the OK message
 */
public class BSUtilsTestOK {

    @Test
    public void message_OK_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            Random random = new Random();

            int randomPlayerId = random.nextInt(89999) + 10000; // Random number between 10000 and 99999
            int randomGameId = random.nextInt(89999) + 10000; // Random number between 10000 and 99999

            // Send data
            bsComUtils.write_OK(randomPlayerId, randomGameId);

            // Read data
            MessageOK message = (MessageOK) bsComUtils.read();


            // Check data consistency
            byte messageType = message.getMessageType();
            int playerId = message.getPlayerId();
            int gameId = message.getGameId();

            // Check data consistency with asserts
            assert messageType == 1; // code 1
            assert playerId > 9999 && playerId < 100000; // 5 digits
            assert gameId > 9999 && gameId < 100000; // 5 digits

            assert playerId == randomPlayerId;
            assert gameId == randomGameId;

            System.out.println("Test OK passed");
            System.out.println("MessageType: " + messageType);
            System.out.println("PlayerID: " + playerId);
            System.out.println("GameID: " + gameId);

        } catch (IOException e) {
            System.out.println("FAILED : Test OK ");
            e.printStackTrace();
        }
    }


}
