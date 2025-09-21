package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageGetConfig;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the GetConfig message
 */
public class BSUtilsTestGetConfig {

    @Test
    public void message_OK_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));


            int playerId = 10000;
            int gameId = 10000;

            // Send data
            bsComUtils.write_GETCONFIG(playerId, gameId);

            // Read data
            MessageGetConfig message = (MessageGetConfig) bsComUtils.read();

            int ReadplayerId = message.getPlayerId();
            int ReadgameId = message.getGameId();

            // Check data consistency with asserts
            assert message.getMessageType() == 5; // code 5

            assert ReadplayerId == playerId;
            assert ReadgameId == gameId;

            System.out.println("Test GET CONFIG passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test GET CONFIG ");
            e.printStackTrace();
        }
    }


}
