package utils.Message;

import utils.ComUtils;
import utils.ShipType;

import java.io.IOException;
import java.io.OutputStream;

public class MessageGameConfig extends MessageAbstract{


    public MessageGameConfig(byte messageType) {
        super(messageType);
    }

    /**
     * W: Width of the board
     */
    private byte W;

    /**
     * H: Height of the board
     */
    private byte H;

    /**
     * T1: Number of ships of size 1
     */
    private byte T1;

    /**
     * T2: Number of ships of size 2
     */
    private byte T2;

    /**
     * T3: Number of ships of size 3
     */
    private byte T3;

    /**
     * T4: Number of ships of size 4
     */
    private byte T4;

    /**
     * T5: Number of ships of size 5
     */
    private byte T5;

    /**
     * Reads the GAMECONFIG message from the input stream.
     * body: W (byte), H (byte)
     * T1 (byte), T2 (byte), T3 (byte), T4 (byte), T5 (byte)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.W = comUtils.read_bytes(1)[0];
        this.H = comUtils.read_bytes(1)[0];

        this.T1 = comUtils.read_bytes(1)[0];
        this.T2 = comUtils.read_bytes(1)[0];
        this.T3 = comUtils.read_bytes(1)[0];
        this.T4 = comUtils.read_bytes(1)[0];
        this.T5 = comUtils.read_bytes(1)[0];

        //printConfig();
    }

    public byte getW() {
        return W;
    }

    public byte getH() {
        return H;
    }

    public byte getT1() {
        return T1;
    }

    public byte getT2() {
        return T2;
    }

    public byte getT3() {
        return T3;
    }

    public byte getT4() {
        return T4;
    }

    public byte getT5() {
        return T5;
    }

    public void printConfig() {
        System.out.println("-----------------------");
        System.out.println("Configuració original del joc");
        System.out.println("Nombre columnes: " + W);
        System.out.println("Nombre files: " + H);
        System.out.println("vaixells del tipus " + ShipType.fromCode((byte)1) + " : " + T1);
        System.out.println("vaixells del tipus " + ShipType.fromCode((byte)2) + " : " + T2);
        System.out.println("vaixells del tipus " + ShipType.fromCode((byte)3) + " : " + T3);
        System.out.println("vaixells del tipus " + ShipType.fromCode((byte)4) + " : " + T4);
        System.out.println("vaixells del tipus " + ShipType.fromCode((byte)5) + " : " + T5);
        System.out.println("-----------------------");
    }


}
