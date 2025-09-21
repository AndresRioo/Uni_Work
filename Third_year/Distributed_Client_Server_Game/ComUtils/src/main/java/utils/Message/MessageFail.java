package utils.Message;

import utils.ComUtils;

import java.io.IOException;

public class MessageFail extends MessageAbstract{


    public MessageFail(byte messageType) {
        super(messageType);
    }

    /**
     * Reads the FAIL message from the input stream.
     * @throws IOException
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {
    }



}
