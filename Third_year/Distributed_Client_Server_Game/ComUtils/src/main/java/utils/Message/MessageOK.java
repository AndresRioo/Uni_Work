package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageOK extends MessageAbstract{

    /**
     * playerId: Player identifier
     */
    private int playerId;

    /**
     * gameId: Game identifier
     */
    private int gameId;

    public MessageOK(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the OK message from the input stream.
     * body: playerId (int32), gameId (int32)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.playerId = comUtils.bytesToInt32( comUtils.read_bytes(4) , ComUtils.Endianness.BIG_ENNDIAN);
        this.gameId = comUtils.bytesToInt32( comUtils.read_bytes(4) , ComUtils.Endianness.BIG_ENNDIAN);
    }


    public int getPlayerId() {
        return playerId;
    }

    public int getGameId() {
        return gameId;
    }


}
