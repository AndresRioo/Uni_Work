package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageCreate;
import utils.Message.MessageOK;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the Create message
 */
public class BSUtilsTestCreate {

    @Test
    public void message_OK_test() {
        File file = new File("test");
        try {
            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));


            String PlayerName = "Player1";
            byte W = 10;
            byte H = 20;
            byte T1 = 5;
            byte T2 = 4;
            byte T3 = 3;
            byte T4 = 2;
            byte T5 = 1;
            boolean IA = false;

            // Send data
            bsComUtils.write_CREATE( PlayerName, W, H, T1, T2, T3, T4, T5, IA );

            // Read data
            MessageCreate message = (MessageCreate) bsComUtils.read();



            // Check data consistency with asserts
            assert message.getMessageType() == 2; // code 2 Create

            assert message.getNomJugador().equals(PlayerName);
            assert message.getW() == W;
            assert message.getH() == H;
            assert message.getT1() == T1;
            assert message.getT2() == T2;
            assert message.getT3() == T3;
            assert message.getT4() == T4;
            assert message.getT5() == T5;
            assert message.getIA() == IA;

            System.out.println("Test Create passed");
            System.out.println("Player name: " + message.getNomJugador());
            System.out.println("W: " + message.getW());
            System.out.println("H: " + message.getH());
            System.out.println("T1: " + message.getT1());
            System.out.println("T2: " + message.getT2());
            System.out.println("T3: " + message.getT3());
            System.out.println("T4: " + message.getT4());
            System.out.println("T5: " + message.getT5());
            System.out.println("IA: " + message.getIA());

        } catch (IOException e) {
            System.out.println("FAILED : Test Create ");
            e.printStackTrace();
        }
    }


}
