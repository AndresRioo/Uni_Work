package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageReJoin;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the REJOIN message
 */
public class BSUtilsTestReJoin {

    @Test
    public void message_JOIN_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            String OriginalName = "Player1";

            // Send data
            bsComUtils.write_REJOIN(OriginalName);

            // Read data
            MessageReJoin message = (MessageReJoin) bsComUtils.read();

            String player = message.getNomJugador();

            // Check data consistency with asserts
            assert message.getMessageType() == 4; // code 4 ReJoin


            assert player.equals(OriginalName);

            System.out.println("Test Rejoin passed");

        } catch (IOException e) {

            System.out.println("FAILED : Test Rejoin ");
            e.printStackTrace();
        }
    }




}
