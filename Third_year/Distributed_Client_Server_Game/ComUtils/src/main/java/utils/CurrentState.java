package utils;

/**
 * Enum for the different states of the game.
 */
public enum CurrentState {

    /**
     * The game is waiting for players to join (when we create the game or join a game without a player 2)
     */
    WAITING_PLAYERS((byte)1),

    /**
     * The game is in the setup phase (Adding ships to board).
     */
    SETUP((byte)2),

    /**
     * The game is currently being played.
     */
    PLAYING((byte)3),

    /**
     * The game has finished.
     */
    FINISHED((byte)4);

    /**
     * The code of the state.
     */
    private final byte code;

    CurrentState(byte code) {
        this.code = code;
    }

    public byte getCode() {
        return code;
    }

    /**
     * Returns the state from the code.
     * @param code The code of the state.
     * @return The state.
     */
    public static CurrentState fromCode(byte code) {
        for (CurrentState state : CurrentState.values()) {
            if (state.code == code) {
                return state;
            }
        }
        return null;
    }
}
