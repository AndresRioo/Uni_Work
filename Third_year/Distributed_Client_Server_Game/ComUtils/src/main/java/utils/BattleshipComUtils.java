package utils;

import utils.Message.*;

import java.io.*;
import java.net.Socket;

/**
 * Utility class for communication, taking care of in-game messages.
 */
public class BattleshipComUtils extends ComUtils{

    Socket socket;

    /**
     * Initializes the communication utilities with input and output streams.
     * @param inputStream input stream
     * @param outputStream output stream
     * @throws IOException if an I/O error occurs
     */
    public BattleshipComUtils(InputStream inputStream, OutputStream outputStream) throws IOException {
        super(inputStream, outputStream);

    }

    /**
     * Copy contructor for BattleshipComUtils.
     *
     * @param obj  Input ComUtils obj.
     */
    public BattleshipComUtils(ComUtils obj){
        super(obj);
    }

    /**
     * Copy contructor for BattleshipComUtils.
     * @param obj Input ComUtils obj.
     * @param socket Socket object.
     */
    public BattleshipComUtils(ComUtils obj, Socket socket){
        super(obj);
        this.socket = socket;
    }

    /**
     * Writes an ERROR message to the output stream.
     * @throws IOException if an I/O error occurs
     */
    public void write_ERROR( byte errorID , int NBytes , String message ) throws IOException {
        write_byte((byte) 0); // ERROR messageType

        write_byte(errorID); // Error ID

        write_int32(NBytes); // Number of bytes of the message
        write_string(message);
    }


    /**
     * Initializes the communication utilities with input and output streams.
     * @param randomPlayerId 5-digit random number
     * @param randomGameId 5-digit random number
     */
    public void write_OK(int randomPlayerId, int randomGameId) throws IOException {

        write_byte((byte) 1); // OK messageType

        write_int32(randomPlayerId);
        write_int32(randomGameId);
    }



    /**
     * Writes a CREATE message to the output stream.
     * @param nomJugador player name
     * @param W width of the board
     * @param H height of the board
     * @param T1 number of ships of type 1
     * @param T2 number of ships of type 2
     * @param T3 number of ships of type 3
     * @param T4 number of ships of type 4
     * @param T5 number of ships of type 5
     * @param IA is the player an AI?
     * @throws IOException if an I/O error occurs
     */
    public void write_CREATE( String nomJugador, byte W , byte H , byte T1 , byte T2 , byte T3, byte T4, byte T5, boolean IA) throws IOException {
        write_byte((byte) 2);

        if ( nomJugador.length() > 50 ) throw new IOException("Nom de jugador massa llarg");

        // Add padding to the name of the player to make it 50 bytes
        String paddedName = AddPadding(nomJugador);

        write_string(paddedName); // Player name

        write_byte(W);
        write_byte(H);

        write_byte(T1);
        write_byte(T2);
        write_byte(T3);
        write_byte(T4);
        write_byte(T5);

        write_byte(IA ? (byte) 1 : (byte) 0);


    }


    /**
     * Writes a JOIN message to the output stream.
     * @param nomJugador player name
     * @throws IOException if an I/O error occurs
     */
    public void write_JOIN( String nomJugador) throws IOException {

        if ( nomJugador.length() > 50 ) throw new IOException("Nom de jugador massa llarg");

        // first write the code 3 with 1 byte and then the string ASCII with a maximum of 50 bytes
        write_byte( (byte) 3 ); // JOIN messageType


        // Add padding to the name of the player to make it 50 bytes
        String paddedName = AddPadding(nomJugador);

        write_string(paddedName); // Player name

    }


    /**
     * Writes a REJOIN message to the output stream.
     * @param nomJugador player name
     * @throws IOException if an I/O error occurs
     */
    public void write_REJOIN( String nomJugador ) throws IOException {

        if ( nomJugador.length() > 50 ) throw new IOException("Nom de jugador massa llarg");

        // first write the code 3 with 1 byte and then the string ASCII with a maximum of 50 bytes
        write_byte( (byte) 4 ); // JOIN messageType

        // Add padding to the name of the player to make it 50 bytes
        String paddedName = AddPadding(nomJugador);

        write_string(paddedName); // Player name
    }

    /**
     * Writes a GETCONFIG message to the output stream.
     * @param playerId player identificator
     * @param gameId game identificator
     * @throws IOException if an I/O error occurs
     */
    public void write_GETCONFIG( int playerId , int gameId) throws IOException {
        write_byte((byte) 5);

        write_int32(playerId);
        write_int32(gameId);
    }

    /**
     * Writes a GAMECONFIG message to the output stream.
     * @param W   width of the board
     * @param H   height of the board
     * @param T1  number of ships of type 1
     * @param T2  number of ships of type 2
     * @param T3  number of ships of type 3
     * @param T4  number of ships of type 4
     * @param T5  number of ships of type 5
     * @throws IOException if an I/O error occurs
     */
    public void write_GAMECONFIG( byte W , byte H , byte T1, byte T2, byte T3, byte T4, byte T5 ) throws IOException {
        write_byte((byte) 6);

        write_byte(W);
        write_byte(H);

        write_byte(T1);
        write_byte(T2);
        write_byte(T3);
        write_byte(T4);
        write_byte(T5);

    }
    /**
     * Writes a ADDVESSEL message to the output stream.
     * @param playerId  player identificator
     * @param gameId  game identificator
     * @param type  number of the type of ship
     * @param ri  initial row
     * @param ci  initial column
     * @param rf  final row
     * @param cf  final column
     * @throws IOException if an I/O error occurs
     */
    public void write_ADDVESSEL(int playerId , int gameId , byte type, byte ri, byte ci, byte rf, byte cf) throws IOException {
        write_byte((byte) 7);

        if (type > 5 || type < 1) throw new IOException("Has d'escollir un tipus de vaixell entre el 1 i el 5");
        if (ri < 1 || rf < 1) throw new IOException("La fila comença a numerar-se a partir del 1");
        if (ci < 1 || cf < 1) throw new IOException("La columna comença a numerar-se a partir del 1");


        write_int32(playerId);
        write_int32(gameId);

        write_byte(type);
        write_byte(ri);
        write_byte(ci);
        write_byte(rf);
        write_byte(cf);
    }
    /**
     * Writes a GETSTATUS message to the output stream.
     * @param playerId  player identificator
     * @param gameId  game identificator
     * @throws IOException if an I/O error occurs
     */
    public void write_GETSTATUS(int playerId , int gameId) throws IOException {
        write_byte((byte) 8);

        write_int32(playerId);
        write_int32(gameId);

    }

    /**
     * Writes a GAMESTATUS message to the output stream.
     * @param gameState game state
     * @param boardSize size of the board
     * @param board1 board of player 1
     * @param board2 board of player 2
     * @param info additional information
     * @throws IOException if an I/O error occurs
     */
    public void write_GAMESTATUS(byte gameState, int boardSize, byte[] board1, byte[] board2, byte[] info ) throws IOException {
        write_byte((byte) 9);

        write_byte(gameState);
        write_int32(boardSize);

        System.out.println("Writing GAMESTATUS message. Board size: " + boardSize);

        for (int i = 0; i < boardSize; i++) {
            write_byte(board1[i]);
        }

        for (int i = 0; i < boardSize; i++) {
            write_byte(board2[i]);
        }
        switch (gameState) {
            case 1:
                break;
            case 2:
                write_byte(info[0]); // Jugador1 preparat
                write_byte(info[1]); // Jugador2 preparat
                write_byte(info[2]); // # Vaixells pendents de posar de tipus T1
                write_byte(info[3]); // # Vaixells pendents de posar de tipus T2
                write_byte(info[4]); // # Vaixells pendents de posar de tipus T3
                write_byte(info[5]); // # Vaixells pendents de posar de tipus T4
                write_byte(info[6]); // # Vaixells pendents de posar de tipus T5
                break;
            case 3:
            case 4:
                write_byte(info[0]); // Torn del Jugador1 o si ha guanyat el Jugador1
                write_byte(info[1]); // Torn del Jugador2 o si ha guanyat el Jugador2
                break;
        }




    }
    /**
     * Writes a SHOT message to the output stream.
     * @param playerId  player identificator
     * @param gameId  game identificator
     * @param r  shot row
     * @param c  shot column
     * @throws IOException if an I/O error occurs
     */
    public void write_SHOT(int playerId , int gameId , byte r, byte c) throws IOException {
        write_byte((byte) 10);

        write_int32(playerId);
        write_int32(gameId);

        write_byte(r);
        write_byte(c);
    }
    /**
     * Writes a HIT message to the output stream.
     * @param sink  true if the ship has sunk
     * @throws IOException if an I/O error occurs
     */
    public void write_HIT(boolean sink) throws IOException {
        write_byte((byte) 11);

        write_byte(sink? (byte) 1 : (byte) 0);

    }
    /**
     * Writes a FAIL message to the output stream.
     * @throws IOException if an I/O error occurs
     */
    public void write_FAIL() throws IOException {
        write_byte((byte) 12);
    }


    /**
     * Writes a LEAVE message to the output stream.
     * @param playerId  player identificator
     * @param gameId  game identificator
     * @throws IOException if an I/O error occurs
     */
    public void write_LEAVE(int playerId , int gameId) throws IOException {
        write_byte((byte) 13);

        write_int32(playerId);
        write_int32(gameId);
    }


    /**
     * Reads the code of the message and calls the corresponding method to read the rest of the message.
     * @throws IOException  if an I/O error occurs
     */
    public MessageAbstract read() throws IOException {

        byte messageType = read_bytes(1)[0];
        MessageAbstract message;

        switch (messageType) {
            case 0: // ERROR
                message = new MessageError(messageType);
                break;
            case 1: // OK
                message = new MessageOK(messageType);
                break;
            case 2: // CREATE
                message = new MessageCreate(messageType);
                break;
            case 3: // JOIN
                message = new MessageJoin(messageType);
                break;
            case 4: // REJOIN
                message = new MessageReJoin(messageType);
                break;
            case 5: // GETCONFIG
                message = new MessageGetConfig(messageType);
                break;
            case 6: // GAMECONFIG
                message = new MessageGameConfig(messageType);
                break;
            case 7: // ADDVESSEL
                message = new MessageAddVessel(messageType);
                break;
            case 8: // GETSTATUS
                message = new MessageGetStatus(messageType);
                break;
            case 9: // GAMESTATUS
                message = new MessageGameStatus(messageType);
                break;
            case 10: // SHOT
                message = new MessageShot(messageType);
                break;
            case 11: // HIT
                message = new MessageHit(messageType);
                break;
            case 12: // FAIL
                message = new MessageFail(messageType);
                break;
            case 13: // LEAVE
                message = new MessageLeave(messageType);
                break;
            default:
                // Not recognized messages
                System.out.println(" ERROR READ BSCOMUTILS : Not recognized :  " + messageType);
                return null;

        }

        message.read(this); // Lee el cuerpo del mensaje
        return message;
    }







    // Auxiliar methods

    /**
     * Adds padding to the player name to make it 50 bytes.
     * @param str byte array to store the player name
     * @return the player name
     */
    private String AddPadding(String str) {
        // Add padding to the name of the player
        StringBuilder paddedName = new StringBuilder(str);
        while (paddedName.length() < 50) {
            paddedName.append('\0'); // Null character
        }
        return paddedName.toString();
    }

    /**
     * Closes the socket.
     * @throws IOException if an I/O error occurs
     */
    public void closeSocket() throws IOException {
        this.closeStreams();
        if (socket != null) {
            socket.close();
        }

    }



}
