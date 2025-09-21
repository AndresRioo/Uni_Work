package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageCreate extends MessageAbstract{


    /**
     * nomJugador: Name of the player
     */
    private String nomJugador;

    /**
     * W: Width of the board
     */
    private int W;

    /**
     * H: Height of the board
     */
    private int H;

    /**
     * T1: Number of ships of size 1
     */
    private int T1;

    /**
     * T2: Number of ships of size 2
     */
    private int T2;

    /**
     * T3: Number of ships of size 3
     */
    private int T3;

    /**
     * T4: Number of ships of size 4
     */
    private int T4;

    /**
     * T5: Number of ships of size 5
     */
    private int T5;

    /**
     * IA: True if the player is an AI
     */
    private boolean IA;


    public MessageCreate(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the CREATE message from the input stream.
     * body: nomJugador (String 50 bytes)
     * W (byte), H (byte)
     * T1 (byte), T2 (byte), T3 (byte), T4 (byte), T5 (byte)
     * IA (byte)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.nomJugador = comUtils.read_string(50);

        this.W = comUtils.read_bytes(1)[0];
        this.H = comUtils.read_bytes(1)[0];

        this.T1 = comUtils.read_bytes(1)[0];
        this.T2 = comUtils.read_bytes(1)[0];
        this.T3 = comUtils.read_bytes(1)[0];
        this.T4 = comUtils.read_bytes(1)[0];
        this.T5 = comUtils.read_bytes(1)[0];

        this.IA = comUtils.read_bytes(1)[0] == 1;

    }

    public String getNomJugador() {
        return nomJugador;
    }

    public int getW() {
        return W;
    }

    public int getH() {
        return H;
    }

    public int getT1() {
        return T1;
    }

    public int getT2() {
        return T2;
    }

    public int getT3() {
        return T3;
    }

    public int getT4() {
        return T4;
    }

    public int getT5() {
        return T5;
    }

    public boolean getIA() {
        return IA;
    }




}
