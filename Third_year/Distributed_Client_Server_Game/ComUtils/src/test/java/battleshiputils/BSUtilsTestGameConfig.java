package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageGameConfig;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
/**
 * Test class to check the GameConfig message
 */
public class BSUtilsTestGameConfig {

    @Test
    public void message_OK_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));


            byte W = 10;
            byte H = 10;

            byte T1 = 5;
            byte T2 = 4;
            byte T3 = 3;
            byte T4 = 2;
            byte T5 = 1;

            // Send data
            bsComUtils.write_GAMECONFIG(W, H, T1, T2, T3, T4, T5);

            // Read data
            MessageGameConfig message = (MessageGameConfig) bsComUtils.read();

            // Check data consistency with asserts
            assert message.getMessageType() == 6; // code 6
            assert message.getW() == W;
            assert message.getH() == H;
            assert message.getT1() == T1;
            assert message.getT2() == T2;
            assert message.getT3() == T3;
            assert message.getT4() == T4;
            assert message.getT5() == T5;


            System.out.println("Test GAME CONFIG passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Game Config ");
            e.printStackTrace();
        }
    }


}
