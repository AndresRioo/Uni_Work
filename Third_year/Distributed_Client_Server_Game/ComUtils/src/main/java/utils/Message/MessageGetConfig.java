package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageGetConfig extends MessageAbstract{

    /**
     * playerId: Id of the player
     */
    private int playerId;

    /**
     * gameId: Id of the game
     */
    private int gameId;

    public MessageGetConfig(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the GETCONFIG message from the input stream.
     * body: playerId (int32), gameId (int32)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.playerId = comUtils.read_int32();
        this.gameId = comUtils.read_int32();

    }

    public int getPlayerId() {
        return playerId;
    }

    public int getGameId() {
        return gameId;
    }


}
