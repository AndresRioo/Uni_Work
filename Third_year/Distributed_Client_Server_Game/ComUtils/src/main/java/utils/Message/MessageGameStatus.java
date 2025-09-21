package utils.Message;

import utils.ComUtils;
import utils.GameInfo;

import java.io.IOException;

public class MessageGameStatus extends MessageAbstract{

    /**
     * gameStatus: Game status object with the information of the game.
     */
    private GameInfo gameStatus;


    public MessageGameStatus(byte messageType) {
        super(messageType);
    }


    /**
     * Reads the GAMESTATUS message from the input stream.
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        gameStatus = new GameInfo(-1); //flag -1 to indicate that the game is not initialized

        try {

            //System.out.println("Reading GAMESTATUS message");

            // Read the game status (1 byte)
            gameStatus.setGameState(comUtils.read_bytes(1)[0]);

            // Read the board size (4 bytes/ 1 int32)
            gameStatus.boardSize = comUtils.bytesToInt32(comUtils.read_bytes(4), ComUtils.Endianness.BIG_ENNDIAN);

            // Read the boards (boardSize bytes)

            //System.out.println("Reading GAMESTATUS message. Board size: " + gameStatus.boardSize);

            gameStatus.boardPlayer1 = comUtils.read_bytes(gameStatus.boardSize);
            gameStatus.boardPlayer2 = comUtils.read_bytes(gameStatus.boardSize);

            switch (gameStatus.getGameState()){
                case WAITING_PLAYERS: // Waiting for players
                    break;
                case SETUP: // Setup
                    gameStatus.isJugador1Preparat = comUtils.read_bytes(1)[0];
                    gameStatus.isJugador2Preparat = comUtils.read_bytes(1)[0];

                    gameStatus.nships1 = comUtils.read_bytes(1)[0];
                    gameStatus.nships2 = comUtils.read_bytes(1)[0];
                    gameStatus.nships3 = comUtils.read_bytes(1)[0];
                    gameStatus.nships4 = comUtils.read_bytes(1)[0];
                    gameStatus.nships5 = comUtils.read_bytes(1)[0];

                    break;
                case PLAYING: // Playing
                    gameStatus.tornJugador1 = comUtils.read_bytes(1)[0];
                    gameStatus.tornJugador2 = comUtils.read_bytes(1)[0];

                    break;
                case FINISHED: // Finished
                    gameStatus.guanyaJugador1 = comUtils.read_bytes(1)[0];
                    gameStatus.guanyaJugador2 = comUtils.read_bytes(1)[0];

                    break;
            }
        } catch (IOException e) {
            System.out.println("Error reading GAMESTATUS message");
            e.printStackTrace();
        }
    }

    public GameInfo getGameInfo() {
        return gameStatus;
    }


}
