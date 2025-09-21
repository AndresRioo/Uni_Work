package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageFail;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
/**
 * Test class to check the FAIL message
 */
public class BSUtilsTestFail {

    @Test
    public void message_FAIL_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));


            // Send data
            bsComUtils.write_FAIL();

            // Read data
            MessageFail message = (MessageFail) bsComUtils.read();


            // Check data consistency with asserts
            assert message.getMessageType() == 12 ; // code 9 Fail



            System.out.println("Test FAIL passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Fail ");
            e.printStackTrace();
        }
    }


}
