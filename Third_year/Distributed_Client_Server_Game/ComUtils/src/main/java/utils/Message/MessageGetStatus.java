package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageGetStatus extends MessageAbstract{




    public MessageGetStatus(byte messageType) {
        super(messageType);
    }

    /**
     * playerId: Player identifier
     */
    private int playerId;

    /**
     * gameId: Game identifier
     */
    private int gameId;


    /**
     * Reads the GETSTATUS message from the input stream.
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
