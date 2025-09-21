package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageHit;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the HIT message
 */
public class BSUtilsTestHit {

    @Test
    public void message_HIT_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            boolean sink= false;


            // Send data
            bsComUtils.write_HIT( sink );

            // Read data
            MessageHit message = (MessageHit) bsComUtils.read();


            // Check data consistency with asserts
            assert message.getMessageType() == 11 ; // code 11 Hit
            assert message.getSink() == sink;




            System.out.println("Test HIT passed");

        } catch (IOException e) {
            System.out.println("FAILED : Test Hit ");
            e.printStackTrace();
        }
    }


}
