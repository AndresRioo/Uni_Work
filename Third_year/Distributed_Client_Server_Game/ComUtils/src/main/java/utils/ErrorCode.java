package utils;

/**
 * Enum for the different error codes.
 */
public enum ErrorCode {
    UNKNOWN_ERROR((byte) 0, "Error desconegut"),
    PLAYER_NAME_EXISTS((byte) 1, "El nom de jugador ja existeix"),
    INVALID_PLAYER_NAME((byte) 2, "El nom de jugador és invàlid (no més de 50 caràcters i no pot ser buit)"),
    INVALID_GAME_ID((byte) 3, "L'identificador de partida no és vàlid"),
    INVALID_PLAYER_ID((byte) 4, "L'identificador de jugador no és vàlid"),
    GAME_UNAVAILABLE((byte) 5, "Partida no disponible"),
    INVALID_GAME_PARAMETERS((byte) 6, "Paràmetres de partida incorrectes"),
    INVALID_LENGTH((byte) 7, "Longitud incorrecta"),
    TYPE_UNAVAILABLE((byte) 8, "Tipus no disponible"),
    INVALID_COORDINATE((byte) 9, "Coordenada incorrecta"),
    INVALID_STATE((byte) 10, "Estat incorrecte");

    /**
     * errorId: Error identifier
     */
    private final byte errorId;

    /**
     * description: Error description
     */
    private final String description;

    ErrorCode(byte errorId, String description) {
        this.errorId = errorId;
        this.description = description;
    }

    public byte getErrorId() {
        return errorId;
    }

    public String getDescription() {
        return description;
    }

    /**
     * Returns the error from the error identifier.
     * @param errorId The error identifier.
     * @return The error.
     */
    public static ErrorCode fromId(byte errorId) {
        for (ErrorCode error : values()) {
            if (error.errorId == errorId) {
                return error;
            }
        }
        return UNKNOWN_ERROR; // Si no se encuentra, devolver error desconocido
    }

    @Override
    public String toString() {
        return errorId + ": " + description;
    }
}
