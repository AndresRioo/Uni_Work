package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageJoin;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the JOIN message
 */
public class BSUtilsTestJoin {

    @Test
    public void message_JOIN_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            String OriginalName = "Player1";

            // Send data
            bsComUtils.write_JOIN(OriginalName);

            // Read data
            MessageJoin message = (MessageJoin) bsComUtils.read();

            String player = message.getNomJugador();

            // Check data consistency with asserts
            assert message.getMessageType() == 3; // code 3 Join

            assert player.equals(OriginalName);

            System.out.println("Test Join Passed");

        } catch (IOException e) {
            System.out.println("FAILED : Test JOIN ");
            e.printStackTrace();
        }
    }


}
