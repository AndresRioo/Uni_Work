package battleshiputils;

import org.junit.Test;
import utils.BattleshipComUtils;
import utils.Message.MessageGameStatus;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test class to check the GameStatus message
 */
public class BSUtilsTestGameStatus {

    @Test
    public void message_GameStatus_1_test() {

        File file = new File("test");
        try {


            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            byte gameState = 1;
            int boardSize = 25;  // Resultado de W*H (5*5)
            byte[] board1 = new byte[boardSize];
            byte[] board2 = new byte[boardSize];

            for (int i = 0; i < boardSize; i++) {
                board1[i] = 0;
                board2[i] = 0;
            }




            // Send data
            bsComUtils.write_GAMESTATUS( gameState, boardSize, board1, board2 , null  );

            // Read data
            MessageGameStatus message = (MessageGameStatus) bsComUtils.read();

            // Check data consistency with asserts
            assert message.getMessageType() == 9 ; // code 9 GameStatus
            assert message.getGameInfo().getGameState().getCode() == gameState;
            assert message.getGameInfo().boardSize == boardSize;

            System.out.println("board1.length: " + message.getGameInfo().boardPlayer1.length);
            System.out.println("board2.length: " + message.getGameInfo().boardPlayer2.length);
            System.out.println("boardSize: " + boardSize);
            System.out.println("boardSize*boardSize: " + boardSize*boardSize);


            assert message.getGameInfo().boardPlayer1.length == boardSize;
            assert message.getGameInfo().boardPlayer2.length == boardSize;

            System.out.println("Test GameStatus 1 passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Game Status 1 ");
            e.printStackTrace();
        }
    }

    @Test
    public void message_GameStatus_2_test() {

        File file = new File("test");
        try {


            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            byte gameState = 2;
            int boardSize = 10;
            byte[] board1 = new byte[boardSize];
            byte[] board2 = new byte[boardSize];

            for (int i = 0; i < boardSize; i++) {
                board1[i] = 0;
                board2[i] = 0;
            }

            byte isJugador1Preparat = 1;
            byte isJugador2Preparat = 1;
            byte nships1 = 1;
            byte nships2 = 1;
            byte nships3 = 1;
            byte nships4 = 1;
            byte nships5 = 1;

            byte[] data = new byte[7];
            data[0] = isJugador1Preparat;
            data[1] = isJugador2Preparat;
            data[2] = nships1;
            data[3] = nships2;
            data[4] = nships3;
            data[5] = nships4;
            data[6] = nships5;





            // Send data
            bsComUtils.write_GAMESTATUS( gameState, boardSize, board1, board2 , data  );

            // Read data
            MessageGameStatus message = (MessageGameStatus) bsComUtils.read();

            // Check data consistency with asserts
            assert message.getMessageType() == 9 ; // code 9 GameStatus
            assert message.getGameInfo().getGameState().getCode() == gameState;
            assert message.getGameInfo().boardSize == boardSize;
            assert message.getGameInfo().boardPlayer1.length == boardSize;
            assert message.getGameInfo().boardPlayer2.length == boardSize;

            assert message.getGameInfo().isJugador1Preparat == isJugador1Preparat;
            assert message.getGameInfo().isJugador2Preparat == isJugador2Preparat;
            assert message.getGameInfo().nships1 == nships1;
            assert message.getGameInfo().nships2 == nships2;
            assert message.getGameInfo().nships3 == nships3;
            assert message.getGameInfo().nships4 == nships4;
            assert message.getGameInfo().nships5 == nships5;


            System.out.println("Test GameStatus 2 passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Game Status 2 ");
            e.printStackTrace();
        }
    }

    @Test
    public void message_GameStatus_3_test() {

        File file = new File("test");
        try {


            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            byte gameState = 3;
            int boardSize = 10;
            byte[] board1 = new byte[boardSize];
            byte[] board2 = new byte[boardSize];

            for (int i = 0; i < boardSize; i++) {
                board1[i] = 0;
                board2[i] = 0;
            }

            byte tornJugador1 = 1;
            byte tornJugador2 = 1;

            byte[] data = new byte[2];
            data[0] = tornJugador1;
            data[1] = tornJugador2;






            // Send data
            bsComUtils.write_GAMESTATUS( gameState, boardSize, board1, board2 , data  );

            // Read data
            MessageGameStatus message = (MessageGameStatus) bsComUtils.read();

            // Check data consistency with asserts
            assert message.getMessageType() == 9 ; // code 9 GameStatus
            assert message.getGameInfo().getGameState().getCode() == gameState;
            assert message.getGameInfo().boardSize == boardSize;
            assert message.getGameInfo().boardPlayer1.length == boardSize;
            assert message.getGameInfo().boardPlayer2.length == boardSize;

            assert message.getGameInfo().tornJugador1 == tornJugador1;
            assert message.getGameInfo().tornJugador2 == tornJugador2;



            System.out.println("Test GameStatus 3 passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Game Status 3 ");
            e.printStackTrace();
        }
    }

    @Test
    public void message_GameStatus_4_test() {

        File file = new File("test");
        try {


            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            byte gameState = 4;
            int boardSize = 10;
            byte[] board1 = new byte[boardSize];
            byte[] board2 = new byte[boardSize];

            for (int i = 0; i < boardSize; i++) {
                board1[i] = 0;
                board2[i] = 0;
            }

            byte haGuanyatJugador1 = 1;
            byte haGuanyatJugador2 = 0;


            byte[] data = new byte[2];
            data[0] = haGuanyatJugador1;
            data[1] = haGuanyatJugador2;






            // Send data
            bsComUtils.write_GAMESTATUS( gameState, boardSize, board1, board2 , data  );

            // Read data
            MessageGameStatus message = (MessageGameStatus) bsComUtils.read();

            // Check data consistency with asserts
            assert message.getMessageType() == 9 ; // code 9 GameStatus
            assert message.getGameInfo().getGameState().getCode() == gameState;
            assert message.getGameInfo().boardSize == boardSize;
            assert message.getGameInfo().boardPlayer1.length == boardSize;
            assert message.getGameInfo().boardPlayer2.length == boardSize;

            assert message.getGameInfo().guanyaJugador1 == haGuanyatJugador1;
            assert message.getGameInfo().guanyaJugador2 == haGuanyatJugador2;


            System.out.println("Test GameStatus 4 passed");


        } catch (IOException e) {
            System.out.println("FAILED : Test Game Status 4 ");
            e.printStackTrace();
        }
    }


}