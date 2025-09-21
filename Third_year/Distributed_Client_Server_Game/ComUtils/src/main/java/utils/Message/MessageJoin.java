package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageJoin extends MessageAbstract{

    /**
     * nomJugador: Name of the player
     */
    private String nomJugador;


    /**
     * Constructor
     * @param messageType
     */
    public MessageJoin(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the JOIN message from the input stream.
     * body: nomJugador (String 50 bytes)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.nomJugador = comUtils.read_string(50);

    }

    public String getNomJugador() {
        return nomJugador;
    }


}
