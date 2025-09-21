package utils.Message;

import utils.ComUtils;
import utils.ErrorCode;

import java.io.IOException;

public class MessageError extends MessageAbstract{

    /**
     * errorID: Error identifier
     */
    private ErrorCode error;

    /**
     * errorDescription: Error description
     */
    private String errorDescription;

    public MessageError(byte messageType) {
        super(messageType);
    }

    /**
     * Read the error message from the input stream
     *
     */
    @Override
    public void read(ComUtils comUtils) throws IOException {
        // Lee el errorID como byte
        byte errorId = comUtils.read_bytes(1)[0];

        // Obtiene el error del enum
        this.error = ErrorCode.fromId(errorId);

        // Lee la longitud del mensaje de error
        int length = comUtils.read_int32();

        // Lee la descripción del error
        this.errorDescription = comUtils.read_string(length);
    }

    public ErrorCode getError() {
        return error;
    }

    public String getErrorDescription() {
        return errorDescription;
    }

    public byte getErrorId() {
        return error.getErrorId();
    }




}
