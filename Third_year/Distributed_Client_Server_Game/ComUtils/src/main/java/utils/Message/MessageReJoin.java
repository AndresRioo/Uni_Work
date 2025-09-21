package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageReJoin extends MessageAbstract{

    /**
     * nomJugador: Name of the player
     */
    private String nomJugador;

    public MessageReJoin(byte messageType) {
        super(messageType);
    }

    public String getNomJugador() {
        return nomJugador;
    }

    /**
     * Reads the REJOIN message from the input stream.
     * body: nomJugador (String 50 bytes)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.nomJugador = comUtils.read_string(50);

    }


}
