package SetupPhase;

import org.junit.Test;
import p1.server.GameHandler;
import utils.BattleshipComUtils;
import utils.CurrentState;
import utils.GameException;
import utils.Message.MessageAddVessel;
import utils.Message.MessageShot;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Test for the shot method (bot shot)
 */
public class ShotBotTest {

    private GameHandler gameHandler;

    private int playerId = 55555;
    private int bot = -1;
    private int gameId = 55555;




    private void createGamehandler(){

        File file = new File("test");
        try {

            file.createNewFile();
            BattleshipComUtils bsComUtils = new BattleshipComUtils(new FileInputStream(file), new FileOutputStream(file));

            GameHandler gameHandler = new GameHandler(bsComUtils);
            gameHandler.initGameInfoFIXEDVALUES(playerId, gameId);
            gameHandler.getGame().setGameState(CurrentState.SETUP);
            gameHandler.getGame().getGameInfo().setWandH((byte) 3, (byte) 3);
            gameHandler.setPlayer2ID(-1);

            boolean checkAddVessel = gameHandler.getBattleShipGame().addVessel(playerId, 1, 1, 2, 1, 5);
            assert checkAddVessel;

            gameHandler.getGame().setOriginalBoard1( gameHandler.getBattleShipGame().getGame().getGameInfo().boardPlayer1.clone() );

            gameHandler.getGame().setGameState(CurrentState.PLAYING);


            this.gameHandler = gameHandler;


        } catch (Exception e) {
            System.out.println("FAILED : createGamehandler in shot error test");
            e.printStackTrace();
        }
    }

    /**
     * Test to check the shot method from the bot
     */
    @Test
    public void shot_bot(){

        createGamehandler();

        gameHandler.getGame().getGameInfo().tornJugador1 = 0;
        gameHandler.getGame().getGameInfo().tornJugador2 = 1; // Player 2 turn

        int result = gameHandler.getBattleShipGame().shot(-1);

        assert result == 1 || result == 0 || result == 2;

        System.out.println("After bot shot  ");

        gameHandler.getGame().getGameInfo().printBoard1();

        System.out.println("----------------------");
        System.out.println("Original board");

        gameHandler.getGame().printOriginalBoard1();

        assert gameHandler.getGame().getOriginalBoard1() != gameHandler.getGame().getGameInfo().boardPlayer1;


    }
}
