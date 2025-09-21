package utils.Message;

import utils.ComUtils;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Abstract class for all type of messages
 * Has a messageType and a read method
 */
public abstract class MessageAbstract {

    /**
     * Code of the message
     */
    protected byte messageType;

    public MessageAbstract(byte messageType) {
        this.messageType = messageType;
    }


    /**
     * Reads the message from the ComUtils
     * @throws IOException
     */
    public abstract void read(ComUtils comUtils) throws IOException;

    public byte getMessageType() {
        return messageType;
    }
}

