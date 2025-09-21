package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageError;
import utils.ErrorCode;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the Error message
 */
public class BSUtilsTestError {

    @Test
    public void message_Error_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));


            // Exemple of Unknown Error
            byte errorID = ErrorCode.UNKNOWN_ERROR.getErrorId();
            int length = ErrorCode.UNKNOWN_ERROR.getDescription().length();
            String errorDescription = ErrorCode.UNKNOWN_ERROR.getDescription();

            // Send data
            bsComUtils.write_ERROR( errorID , length ,  errorDescription );

            // Read data
            MessageError message = (MessageError) bsComUtils.read();

            // Check data consistency with asserts
            assert message.getMessageType() == 0; // code 0 Error
            assert message.getErrorId() == errorID;
            assert message.getErrorDescription().equals(errorDescription);

            System.out.println("Test ERROR passed");

        } catch (IOException e) {
            System.out.println("FAILED : Test Error ");
            e.printStackTrace();
        }
    }


}
