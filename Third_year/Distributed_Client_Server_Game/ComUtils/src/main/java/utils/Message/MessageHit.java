package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

public class MessageHit extends MessageAbstract{

    /**
     * sink: True if the hit sunk a vessel, false otherwise
     */
    private boolean sink;

    public MessageHit(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the SHOT message from the input stream.
     * body: sink (Byte)
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {

        this.sink = comUtils.read_bytes(1)[0] == 1;

    }

    public boolean getSink() {
        return sink;
    }


}
