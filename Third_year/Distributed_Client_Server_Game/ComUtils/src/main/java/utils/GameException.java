package utils;

/**
 * Custom exception for the game, to be thrown when an error in game occurs.
 */
public class GameException extends Exception{

    public GameException(String message) {
        super(message);
    }
}
