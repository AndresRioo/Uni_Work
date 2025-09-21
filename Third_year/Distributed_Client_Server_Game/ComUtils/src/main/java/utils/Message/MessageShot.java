package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageShot extends MessageAbstract{

    /**
     * playerId: Player identifier
     */
    private int playerId;

    /**
     * gameId: Game identifier
     */
    private int gameId;

    /**
     * r: Row index of the shot
     */
    private byte r;

    /**
     * c: Column index of the shot
     */
    private byte c;

    public MessageShot(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the SHOT message from the input stream.
     * body: playerId (int32), gameId (int32)
     * type (byte), ci (byte), ri (byte), cf (byte), rf (byte)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.playerId = comUtils.read_int32();
        this.gameId = comUtils.read_int32();
        this.r = comUtils.read_bytes(1)[0];
        this.c = comUtils.read_bytes(1)[0];

        r--;
        c--; // 0 indexed

        System.out.println("Message Shot: PlayerId: " + playerId + " GameId: " + gameId + " Row: " + r + " Column: " + c );

    }

    public int getPlayerId() {
        return playerId;
    }

    public int getGameId() {
        return gameId;
    }

    public byte getR() {
        return r;
    }

    public byte getC() {
        return c;
    }



}
