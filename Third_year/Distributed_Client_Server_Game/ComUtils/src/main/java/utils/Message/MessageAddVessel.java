package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageAddVessel extends MessageAbstract{



    public MessageAddVessel(byte messageType) {
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
     * type: Type of the vessel
     */
    private byte type;

    /**
     * ri: Row index of the vessel
     */
    private byte ri;

    /**
     * ci: Column index of the vessel
     */
    private byte ci;

    /**
     * rf: Final row index of the vessel
     */
    private byte rf;

    /**
     * cf: Final column index of the vessel
     */
    private byte cf;



    /**
     * Reads the ADDVESSEL message from the input stream.
     * body: playerId (int32), gameId (int32)
     * type (byte), ci (byte), ri (byte), cf (byte), rf (byte)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.playerId = comUtils.read_int32();
        this.gameId = comUtils.read_int32();
        this.type = comUtils.read_bytes(1)[0];
        this.ri = comUtils.read_bytes(1)[0];
        this.ci = comUtils.read_bytes(1)[0];
        this.rf = comUtils.read_bytes(1)[0];
        this.cf = comUtils.read_bytes(1)[0];

        System.out.println("Message AddVessel: PlayerId: " + playerId + " GameId: " + gameId + " Type: " + type + " Row: " + ri + " Column: " + ci + " Final Row: " + rf + " Final Column: " + cf);
    }

    public byte getType() {
        return type;
    }

    public byte getRi() {
        return ri;
    }

    public byte getCi() {
        return ci;
    }

    public byte getRf() {
        return rf;
    }

    public byte getCf() {
        return cf;
    }

    public int getPlayerId() {
        return playerId;
    }

    public int getGameId() {
        return gameId;
    }




}
