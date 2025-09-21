package p1.client;

import utils.*;
import utils.Message.*;

import java.io.IOException;
import java.util.Scanner;

/**
 * GameClient class that encapsulates the game's logic.
 * It follows a sequence of states as per the established protocol.
 */
public class GameClient {

    /**
     * Communication utility for interacting with the server.
     */
    BattleshipComUtils bsComUtils;

    /**
     * Player's name.
     */
    String nomJugador;

    /**
     * Player's ID.
     */
    int playerId;

    /**
     * Game's ID.
     */
    int gameId;

    /**
     * Scanner for reading user input.
     */
    Scanner sc = new Scanner(System.in);

    /**
     * Game information (Size, ships, boards, etc...)
     */
    GameInfo gameInfo;



    /**
     * Initializes the game handler with a communication utility.
     *
     * @param comutils The communication utility for interacting with the server.
     */
    public GameClient(BattleshipComUtils comutils) {
        this.bsComUtils = comutils;
    }

    /**
     * Starts the game client.
     * This method will be responsible for initializing and managing the game client session.
     *
     *  @throws IOException If there is an error in communication with the server
     */
    public void start() throws IOException, GameException {

        menuInici(); // ask name and send join

        System.out.println("Has entrat a la partida amb el jugadorId: " + playerId + " i gameId: " + gameId);

        MessageAbstract FirstMessage = bsComUtils.read();
        MessageGameStatus status;

        // ok was received previously, so the next message should be a game status (can't be a error)
        if (FirstMessage instanceof MessageError) {
            MessageError error = (MessageError) FirstMessage;
            System.out.println("Error received from server : " + error.getErrorDescription());
            closeConnection();
            System.exit(1);

        } else if (FirstMessage instanceof MessageGameStatus) {
            status = (MessageGameStatus) FirstMessage;
            gameInfo = new GameInfo(-1);
            gameInfo.setNewGameInfo(status.getGameInfo());

        } else {
            System.out.println("UNEXPECTED ERROR (start) : Unexpected message type --> " + FirstMessage.getMessageType());
            closeConnection();
            System.exit(1);
        }

        sendGetConfig();

        System.out.println("-----------AFEGEIX ELS TEUS VAIXELLS-----------------");
        afegirVaixells(); // state 2 (SETUP)
        System.out.println("-----------COMENÇA LA PARTIDA!-----------------");
        partida(); // state 3 (PLAYING)
        System.out.println("-----------FINAL DE LA PARTIDA------------------");
        resultados(); // state 4 (FINISHED)

    }


    /**
     * Phase 1: Add ships to the board
     * @throws IOException If there is an error in communication with the server
     * @throws GameException If there is an error in the game
     */
    private void afegirVaixells() throws IOException, GameException {

        if (gameInfo.getGameState() != CurrentState.SETUP){
            System.out.println("ERROR (afegir Vaixells) : Unexpected game state");
            System.out.println("Game state: " + gameInfo.getGameState());
            closeConnection();
            System.exit(1);
        }

        MessageGameStatus status;

        while( gameInfo.getGameState() == CurrentState.SETUP ) {

            if (gameInfo.getNshipsRemainingTotal() != 0) {
                // if there are still ships to add, we enter the menuConfigAddShip()
                menuConfigAddShip();
            } else {
                // no li queden vaixells per afegir, el proxim message ha de ser un MessageGameStatus
                System.out.println("No hi ha més vaixells per afegir, esperant al server...");
            }

            MessageAbstract message = bsComUtils.read();

            // if message is OK, then we can continue adding ships and decrease the number of ships remaining
            if (message instanceof MessageOK) {
                gameInfo.decreaseNShips(gameInfo.getTypeSent());

                // if message is GAMESTATUS, then we can continue the game changing the state to (PLAYING)
            } else if (message instanceof MessageGameConfig) {

                MessageGameConfig config = (MessageGameConfig) message;
                config.printConfig();


            } else if (message instanceof MessageGameStatus) {
                status = (MessageGameStatus) message;
                //System.out.println("GameStatus received");

                if (status.getGameInfo().getGameState() == CurrentState.PLAYING) {
                    System.out.println("Tots els vaixells afegits, comencem a jugar!");
                } else {
                    gameInfo.printInfo(CurrentState.SETUP); // setup not ended , but the player asked for the status
                }
                gameInfo.setNewGameInfo(status.getGameInfo());

                // if message is ERROR, then we have to show the error and try again
            } else if (message instanceof MessageError) { // Error received
                MessageError error = (MessageError) message;
                System.out.println("Error received from server : " + error.getErrorDescription());

                // TODO: Handle the different error codes
                if (error.getError() == ErrorCode.INVALID_GAME_ID) {
                    // error with the game id, we have to leave the game
                    closeConnection();
                } else if (error.getError() == ErrorCode.INVALID_PLAYER_ID) {
                    // error with the player id, we have to leave the game
                    closeConnection();
                } else if (error.getError() == ErrorCode.INVALID_LENGTH) {
                    // As we didn't receive the ok, the remaining ships keep the same
                    System.out.println("ERROR : Invalid length");
                } else if (error.getError() == ErrorCode.TYPE_UNAVAILABLE) {
                    // As we didn't receive the ok, the remaining ships keep the same
                    System.out.println("ERROR : Type unavailable");
                } else if (error.getError() == ErrorCode.INVALID_COORDINATE) {
                    // As we didn't receive the ok, the remaining ships keep the same
                    System.out.println("ERROR : Invalid coordinate");
                } else if (error.getError() == ErrorCode.INVALID_STATE) {
                    // As we didn't receive the ok, the remaining ships keep the same
                    System.out.println("ERROR : Invalid state");
                } else {
                    System.out.println("UNEXPECTED ERROR (afegir vaixell message error) : Unexpected error code --> " + error.getErrorDescription());
                }
            } else {
                System.out.println("UNEXPECTED ERROR (afegir vaixell) : Unexpected message type --> " + message.getMessageType());
            }
        }
    }


    /**
     * Phase 2: Play the game (send shots)
     * @throws IOException If there is an error in communication with the server
     * @throws GameException If there is an error in the game
     */
    private void partida() throws IOException, GameException{

        if (gameInfo.getGameState() != CurrentState.PLAYING){
            System.out.println("ERROR : Unexpected game state");
            System.out.println("Game state: " + gameInfo.getGameState());
            closeConnection();
            System.exit(1);
        }

        while(  gameInfo.getGameState() == CurrentState.PLAYING  ){

            // if it's the turn of the other player, we have to wait
            if (gameInfo.tornJugador1 == 0) {

                int resultat = waitForTheOtherPlayer();

                if (resultat == -1) {
                    continue;
                    // if the user doesn't play, we continue waiting
                    // or the state is finished and we end the loop
                }
            }

            System.out.println("Vols veure l'estat de la partida? (S/N)");
            String resposta = sc.next();

            if (resposta.equalsIgnoreCase("s")) {
                // as we recieve the game status, we can print the boards without asking for the info
                // getconfig is not needed (doesn't make sense to ask for the config in the middle of the game)

                System.out.println("Estat del tauler propi:");
                gameInfo.printBoard1();

                System.out.println("Estat del tauler rival:");
                gameInfo.printBoard2();
            }

            System.out.println("Shot (1) o sortir (2)?");

            if (opcio1o2() == 2){
                leaveRoutine();
            }

            EnviarShot();
            MessageAbstract resultShot = bsComUtils.read();

            if (resultShot instanceof MessageHit) {

                MessageHit hit = (MessageHit) resultShot;
                boolean sink = hit.getSink();

                if (sink) {
                    System.out.println("Vaixell enfonsat!");
                } else {
                    System.out.println("Tret encertat!");
                }

            } else if (resultShot instanceof MessageFail) {

                System.out.println("Tret fallat! Torn del rival");

            } else if (resultShot instanceof MessageError) {

                MessageError error = (MessageError) resultShot;
                System.out.println("Error received from server " + error.getErrorDescription());

                // todo: handle the different error codes
                if (error.getError() == ErrorCode.INVALID_GAME_ID ){ //3
                    System.out.println("ERROR : Invalid game id");
                    throw new GameException("Invalid game id");

                } else if ( error.getError() == ErrorCode.INVALID_PLAYER_ID) { // 4
                    System.out.println("ERROR : Invalid player id");
                    throw new GameException("Invalid player id");

                } else if ( error.getError() == ErrorCode.INVALID_COORDINATE) { // 9
                    System.out.println("ERROR : Invalid coordinate, try again!");
                    continue; // try again the shot
                } else if ( error.getError() == ErrorCode.INVALID_STATE) { // 10
                    if (gameInfo.isThereAnyShipRemaining()){ // if there are still ships to add, we are in SETUP
                        gameInfo.setGameState(CurrentState.SETUP);
                    } else { // if we are not playing anymore, we are in FINISHED
                        gameInfo.setGameState(CurrentState.FINISHED);
                    }
                    continue;
                } else {
                    System.out.println("Not possible Error received from server : " + error.getErrorDescription());
                    closeConnection();
                    System.exit(1);
                }
            } else {
                System.out.println("UNEXPECTED ERROR : Unexpected message type while playing (post sending shot) --> " + resultShot.getMessageType() + " expected : HIT, FAIL or ERROR");
                System.out.println("Message type received : " + resultShot.getMessageType());
                closeConnection();
                System.exit(1);
            }


            // read the state of the game post shot
            MessageGameStatus estatActual = (MessageGameStatus) bsComUtils.read();
            gameInfo.setNewGameInfo(estatActual.getGameInfo());


        }
    }

    /**
     * Game has ended and we have to show the results
     * @throws IOException If there is an error in communication with the server
     * @throws GameException If there is an error in the game
     */
    private void resultados() throws IOException, GameException{

        if (gameInfo.getGameState() != CurrentState.FINISHED){
            System.out.println("ERROR : Unexpected game state");
            System.out.println("Game state: " + gameInfo.getGameState());
            closeConnection();
            System.exit(1);
        }

        if (gameInfo.getGuanyaJugador1() == 1) {
            System.out.println("Has guanyat!");
        }

        if (gameInfo.getGuanyaJugador2() == 1) {
            System.out.println("Has perdut!");
        }

        if (gameInfo.getGuanyaJugador1() == 0 && gameInfo.getGuanyaJugador2() == 0) {
            System.out.println("ERROR : No hi ha guanyador");
        } else if ( gameInfo.getGuanyaJugador1() == 1 && gameInfo.getGuanyaJugador2() == 1) {
            System.out.println("ERROR : Hi ha dos guanyadors");

        }

        System.out.println("Estat final de la partida");
        System.out.println("Tauler propi:");
        gameInfo.printBoard1();
        System.out.println("Tauler rival:");
        gameInfo.printBoard2();

        leaveRoutine();

    }


    /**
     * Display the initial menu.
     * Allowing the player to join a game or exit the game
     *
     *  @throws IOException If there is an error in communication with the server
     */

    public void menuInici() throws IOException , GameException {
        System.out.println("Benvingut a Enfonsar la Flota! \nQuin nom vols utilitzar durant el joc? ");
        // clean buffer
        nomJugador = sc.nextLine();

        if (nomJugador.length() >= 50) {
            System.out.println("nom massa llarg, torna a intentar!");
            menuInici();
            return;
        }

        System.out.println("Selecciona una acció del menu d'opcions:");
        System.out.println("1- Unir-se a una partida");
        System.out.println("2- Crear una partida");
        System.out.println("3- Sortir");

        int opcio = opcioByte();

        switch (opcio) {
            case 1:

                System.out.println("Unint-se...");
                bsComUtils.write_JOIN(nomJugador);
                MessageAbstract message = bsComUtils.read();

                if (message instanceof MessageError) {
                    MessageError error = (MessageError) message;
                    System.out.println("Error received from server : " + error.getErrorDescription());

                    if (error.getError() == ErrorCode.INVALID_PLAYER_NAME) {
                        System.out.println("ERROR : Select a new game name");
                        menuInici();
                        return;
                    }

                    closeConnection();
                    System.exit(1);

                } else if (message instanceof MessageOK) {
                    MessageOK ok = (MessageOK) message;
                    gameId = ok.getGameId();
                    playerId = ok.getPlayerId();

                } else {
                    System.out.println("UNEXPECTED ERROR : Unexpected message type -->" + message.getMessageType());
                    System.exit(1);
                }

                break;
            case 2:
                menuCrearPartida();
                break;
            case 3:
                // never sent the join message, so we don't have to send the leave message
                System.out.println("Sortint del joc...");
                closeConnection();
                System.exit(0);

                break;
            case 4:

                // este caso es para hacer pruebas con el servidor de la asignatura
                // el cliente no deberia de poner 4 y si lo pone es para hacer pruebas
                System.out.println("TEST SERVER DE LA ASIGNATURA (has puesto 4 en el menu, -1 para volver" +
                        ") \nSelecciona una acció del menu d'opcions:");

                int opcio2 = opcioByte();

                switch (opcio2){
                    case 1:
                        System.out.println("Has fet un HIT!");
                        bsComUtils.write_HIT(true);
                        bsComUtils.write_HIT(false);
                        break;
                    case 2:
                        System.out.println("Has fet un FAIL!");
                        bsComUtils.write_FAIL();
                        break;
                    case 3:
                        System.out.println("get status!");
                        bsComUtils.write_GETSTATUS(82832,38492);
                        break;
                    case 4:
                        System.out.println("get config");
                        bsComUtils.write_GETCONFIG(82832,38492);
                        break;
                    case 5:
                        System.out.println("game status");
                        bsComUtils.write_GAMESTATUS((byte)3, 10, new byte[10], new byte[10], new byte[2]);
                        break;
                    case 6:
                        System.out.println("game config");
                        bsComUtils.write_GAMECONFIG((byte)1,(byte)2,(byte)3,(byte)4,(byte)5,(byte)6,(byte)7);
                        break;
                    case 7:
                        System.out.println("add vessel");
                        bsComUtils.write_ADDVESSEL(82832,38492,(byte)1,(byte)2,(byte)3,(byte)4,(byte)5);
                        break;
                    case 8:
                        System.out.println("shot");
                        bsComUtils.write_SHOT(82832,38492,(byte)1,(byte)2);
                        break;
                    case 9:
                        System.out.println("error");
                        bsComUtils.write_ERROR(ErrorCode.INVALID_COORDINATE.getErrorId(), ErrorCode.INVALID_COORDINATE.getDescription().length() ,ErrorCode.INVALID_COORDINATE.getDescription());
                        break;
                    case 10:
                        System.out.println("ok");
                        bsComUtils.write_OK(82832,38492);
                        break;
                    case 11:
                        System.out.println("join");
                        bsComUtils.write_JOIN("pepe");
                        break;
                    case 12:
                        System.out.println("create");
                        bsComUtils.write_CREATE("pepe",(byte)1,(byte)2,(byte)3,(byte)4,(byte)5,(byte)6,(byte)7,false);
                        break;
                    case 13:
                        System.out.println("leave");
                        bsComUtils.write_LEAVE(82832,38492);
                        break;
                    case -1:
                        menuInici();
                        break;
                }


                break;
            default:
                System.out.println("Només hi ha 3 opcions! Torna a intentar");
                menuInici();
                break;
        }

    }


    /**
     * Select the options to create a game
     * @throws IOException If there is an error in communication with the server
     * @throws GameException If there is an error in the game
     */
    private void menuCrearPartida() throws IOException, GameException {

        System.out.println("Selecciona el nombre de files del tauler");
        byte files = opcioByte();

        System.out.println("Selecciona el nombre de columnes del tauler");
        byte columnes = opcioByte();

        System.out.println("Selecciona el nombre de vaixells de tipus LONGSHIP (5 caselles)");
        byte vaixellt1 = opcioByte();

        System.out.println("Selecciona el nombre de vaixells de tipus FRIGATE (4 caselles)");
        byte vaixellt2 = opcioByte();

        System.out.println("Selecciona el nombre de vaixells de tipus BRIG (3 caselles)");
        byte vaixellt3 = opcioByte();

        System.out.println("Selecciona el nombre de vaixells de tipus SCHOONER (3 caselles)");
        byte vaixellt4 = opcioByte();

        System.out.println("Selecciona el nombre de vaixells de tipus SLOOP (2 caselles)");
        byte vaixellt5 = opcioByte();

        System.out.println("Vols jugar contra la IA? (1 - si /2 - no)");
        int ia = opcio1o2();

        System.out.println("Creant partida...");

        bsComUtils.write_CREATE(nomJugador, columnes, files, vaixellt1, vaixellt2, vaixellt3, vaixellt4, vaixellt5, ia != 2);
        MessageAbstract message = bsComUtils.read();

        if (message instanceof MessageError) {

            MessageError error = (MessageError) message;
            if (error.getError() == ErrorCode.INVALID_PLAYER_NAME) {
                System.out.println("ERROR : Invalid player name --> " + error.getErrorDescription());
                menuInici(); // tornar a intentar
                return;
            } else if (error.getError() == ErrorCode.GAME_UNAVAILABLE) {
                System.out.println("La partida no es posible --> " + error.getErrorDescription());
                menuInici(); // tornar a intentar
                return;
            } else {
                System.out.println("ERROR : Unexpected error code --> " + error.getErrorDescription());
                closeConnection();
                System.exit(1);
            }


        } else if (message instanceof MessageOK) {
            MessageOK ok = (MessageOK) message;
            gameId = ok.getGameId();
            playerId = ok.getPlayerId();
        } else {
            System.out.println("UNEXPECTED ERROR : Unexpected message type -->" + message.getMessageType());
            System.exit(1);
        }
    }


    /**
     * Display the configuration menu.
     * Allowing the player to place a ship on the board or exit the game
     *
     * @throws IOException If there is an error in communication with the server
     */
    public void menuConfigAddShip() throws IOException, GameException {
        System.out.println("Selecciona una acció del menu d'opcions:");
        System.out.println("1- Ubicar vaixell");
        System.out.println("2- Veure la configuració original del tauler");
        System.out.println("3- Veure la configuració actual del tauler");
        System.out.println("4- Sortir");

        int opcio = opcioByte();

        switch (opcio) {
            case 1:
                addShip();
                break;
            case 2:
                bsComUtils.write_GETCONFIG(playerId, gameId);
                break;
            case 3:
                bsComUtils.write_GETSTATUS(playerId, gameId);
                break;
            case 4:
                leaveRoutine();
                break;
            default:
                System.out.println("Només hi ha 4 opcions! Torna a intentar");
                menuConfigAddShip();
                break;

        }
    }


    /**
     * Add a ship to the board, asking the user where to place it.
     * Local method to track the state of the board
     * @throws IOException If there is an error in communication with the server
     */
    public void addShip() throws IOException{
        // first find the first available ship and then ask for the coordinates

        byte type = findFirstAvailableShip();

        if (type == -1) {
            System.out.println("ERROR : No type found");
            return;
        }

        // print the info from the board
        printAddVesselSelection(type);

        // ask the coordinates
        byte ri; // Initial row
        byte ci; // Initial column
        byte rf; // Final row
        byte cf; // Final column

        while(true) { // Loop until the user enters a valid position

            System.out.println("Introdueix la columna inicial del vaixell:");
            ci = opcioByte();

            System.out.println("Introdueix la fila inicial del vaixell:");
            ri = opcioByte();


            if (ci < 1 || ri < 1) {
                System.out.println("Posició incorrecte.Coordenades no poder ser inferior a 1. Torna a intentar.");
                continue;
            }

            System.out.println("Vaixell horizontal (1) o vertical (2)?");

            int hv = opcio1o2();

            if (hv == 1) { // Horizontal

                int lastColumn = ci + ShipType.typeToSize(type) - 1;
                cf = (byte) lastColumn;
                rf = ri; // the row is the same

            } else  { // Vertical
                int lastRow = ri + ShipType.typeToSize(type) - 1;
                rf = (byte) lastRow;
                cf = ci; // the column is the same
            }
            // Check if the vessel can be placed in the given coordinates
            if (!checkVessel(type, ri, ci, rf, cf)) {
                continue;
            }
            break; // everything is correct
        }

        System.out.println("Afegint un vaixell amb tipus " + ShipType.fromCode(type) + " i posicions (ri = " + ri + " , ci = " + ci + " , rf = " + rf + " , cf = " + cf +")");
        addVessel(type, ri, ci, rf, cf);

        System.out.println("Vaixell afegit correctament");
        bsComUtils.write_ADDVESSEL(playerId, gameId, type, ri, ci, rf, cf);
    }


    /**
     * Finds the first available ship.
     * @return the type of the first available ship in order from 1 to 5
     */
    public byte findFirstAvailableShip() {
        for (byte type = 1; type <= 5; type++) {
            if (gameInfo.getNShipsRemaining(type) > 0) {
                return type; // Returns the first available ship
            }
        }
        System.out.println("ERROR (findFirstAvailableShip): No more ships available!");
        return -1; // No more ships available, shouldn't be calling this method
    }

    /**
     * Prints the board and the selection of the vessel to add.
     * @param type type of the ship to add (to print the name and size)
     */
    public void printAddVesselSelection(byte type){
        System.out.println("Configuració actual del tauler:");
        gameInfo.printBoard1();
        System.out.println("Selecciona la casella del vaixell del tipus " + ShipType.fromCode(type) + " amb mida " + ShipType.typeToSize(type));
        System.out.println("Fila i columna comença a la posició 1 i acaba a la posició " + gameInfo.W + " (columnes) i " + gameInfo.H + " (files)");
    }

    /**
     * Check if the vessel can be placed in the given coordinates
     * First condition is to check the type of the vessel
     * Then check if the coordinates are within bounds
     * Then check if the vessel is horizontal or vertical
     * Then check if the vessel has the correct size
     * Then check if the vessel is placed in a empty position
     *
     * @param type type of the ship
     * @param ri startign row
     * @param ci starting column
     * @param rf final row
     * @param cf final column
     * @return true if the vessel can be placed in the given coordinates
     */
    public boolean checkVessel( byte type, byte ri, byte ci, byte rf, byte cf){

        ri--; ci--; rf--; cf--; // Adjust the coordinates to 0-based

        // incorrect type
        if (type < 1 || type > 5){
            System.out.println("Tipus de vaixell incorrecte, ha de ser entre 1 i 5. Torna a intentar.");
            return false;
        }
        // Check if the coordinates are within bounds after adjusting (coordinates 0-based)
        // first check if the coordinates don't have negative values
        // then check that the last position is within the board
        if (ri < 0 || ci < 0 || rf < 0 || cf < 0 || rf >= gameInfo.H || cf >= gameInfo.W) {

            if (ri < 0 || ci < 0 || rf < 0 || cf < 0) {
                System.out.println("ERROR: Les coordenades no poden ser negatives. Torna a intentar.");
            } else  {
                System.out.println("ERROR: El vaixell surt fora del tauler! Torna a intentar.");
            }
            return false;
        }
        // incorrect coordinates not in the same row or column
        if (ri != rf && ci != cf){
            System.out.println("El vaixell ha de ser horitzontal o vertical. Torna a intentar.");
            return false;
        }

        int amplada = Math.abs(cf - ci) + 1;  // Number of columns occupied
        int altura = Math.abs(rf - ri) + 1;   // Number of rows occupied

        // Incorrect size
        if (amplada != ShipType.typeToSize(type) && altura != ShipType.typeToSize(type)) {
            System.out.println("El vaixell no té la mida correcta. Ha de ser de mida " + ShipType.typeToSize(type));
            System.out.println("Amplada: " + amplada);
            System.out.println("Altura: " + altura);
            return false;
        }


        // check if the vessel is placed in a empty position
        for (int i = Math.min(ri, rf); i <= Math.max(ri, rf); i++){
            for (int j = Math.min(ci, cf); j <= Math.max(ci, cf); j++){
                if (gameInfo.getBoard1Coords(i, j) != 0){
                    System.out.println("ERROR: La posició : (" + i +","+j+") està ocupada. Torna a intentar.");
                    return false;
                }
            }
        }

        return true;
    }


    /**
     * Add a vessel to the board
     * @param type type of the ship
     * @param ri initial row
     * @param ci initial column
     * @param rf final row
     * @param cf final column
     */
    public void addVessel(byte type, byte ri, byte ci, byte rf, byte cf) {
        // Place the ship on the board
        ri--; ci--; rf--; cf--; // Adjust the coordinates to 0-based

        // Determine the instance number of the ship (1-25)
        byte instanceNumber = gameInfo.getInstanceAndIncrease(type);

        if (instanceNumber > 25) {
            System.out.println("ERROR: Too many ships");
            return;
        }

        // Encode the ship value as per the XYT format

        // X -- Instance number (1-25) but encoded as first digit (*100)
        // Y -- Instance number (1-25) but encoded as second digit (*10)
        // T -- Ship type (1-5) but encoded as third digit (*1)

        byte encodedValue;
        // encode the value

        // first check if the instance number is greater than 12
        // if its not negative, encode the value normally

        // to encode the value, we need to multiply the instance number by 100 so we get the n value (from nnt)
        // then we get the second n from the instance number by getting the remainder of the division by 10
        // then we get the type of the ship and we add it as the unit of the number

        // if its negative we do the same but we need to get the negative value
        // but before that we need to ensure that the instance number is not greater than 13, as the byte can't hold a 13

        if (instanceNumber > 12) { // negative instance number (can't put a 13t in a byte)

            byte valor = 25; // total possible ships
            byte instance = (byte) (valor - instanceNumber); // negative next instance number

            // Encode the normal value
            encodedValue = (byte) ((instance / 10) * 100 + (instance % 10) * 10 + type);

            // get the negative value
            encodedValue = (byte) (encodedValue * -1);

        } else {
            encodedValue =  (byte) ((instanceNumber / 10) * 100 + (instanceNumber % 10) * 10 + type);
        }


        // Horizontal ship: advance by columns
        if (ri == rf) {
            for (int j = ci; j <= cf; j++) {
                gameInfo.setBoard1Coords(ri, j, encodedValue);
            }
        }
        // Vertical ship: advance by rows
        else {
            for (int i = ri; i <= rf; i++) {
                gameInfo.setBoard1Coords(i, ci, encodedValue);
            }
        }


        // Update the number of remaining ships of this type
        //gameInfo.decreaseNShips(type);
        gameInfo.setTypeSent(type);
        // instead of decreasing the number of ships, we set the type sent
        // till we get the ok from the server
    }


    /**
     * Send a shot to the server if not possible error message will be shown
     * @throws IOException If there is an error in communication with the server
     */
    private void EnviarShot() throws IOException {

        System.out.println("Introdueix la columna del tret:");
        byte columna = opcioByte();

        System.out.println("Introdueix la fila del tret:");
        byte fila = opcioByte();

        bsComUtils.write_SHOT(playerId, gameId, fila, columna);
    }


    /**
     * Wait for the other player to play (reads game status)
     * @return -1 if the user doesn't play (finished or hit) or 0 if the user plays
     * @throws IOException If there is an error in communication with the server
     * @throws GameException If there is an error in the game
     */
    private int waitForTheOtherPlayer() throws IOException, GameException {

        System.out.println("-------------------------");
        System.out.println("Torn del altre jugador");

        MessageAbstract messageTurnoRival = bsComUtils.read();

        if (messageTurnoRival instanceof MessageGameStatus){

            MessageGameStatus estatActual = (MessageGameStatus) messageTurnoRival;
            gameInfo.setNewGameInfo(estatActual.getGameInfo());

            if (gameInfo.getGameState() == CurrentState.FINISHED) {
                System.out.println("La partida ha acabat!");
                return -1;
            }

            if (gameInfo.tornJugador1 == 0) {
                System.out.println("El rival ha fet HIT!");
                System.out.println("-------------------------");
                return -1;
            } else {
                System.out.println("El rival ha fet FAIL!");
                System.out.println("-------------------------");
                System.out.println("Comença el teu torn!");
            }

        } else if (messageTurnoRival instanceof MessageError) {

            MessageError error = (MessageError) messageTurnoRival;
            System.out.println("Error received from server : " + error.getErrorDescription());
            System.exit(1);

        } else {
            System.out.println("UNEXPECTED ERROR : Unexpected message type in PLAYING state (waiting for the other player to play) --> " + messageTurnoRival.getMessageType() + " expected : GAMESTATUS or ERROR");
            System.out.println("Message type received : " + messageTurnoRival.getMessageType() + " expected : " );
            System.exit(1);
        }
        return 0;
    }

    /**
     * Close the connection with the server (streams and socket)
     */
    public void closeConnection(){
        try {
            bsComUtils.closeStreams();
            bsComUtils.closeSocket();
            System.out.println("Connection with the server closed correctly");
        } catch (IOException e) {
            System.out.println("Error closing the connection with the server");
            e.printStackTrace();
        }
    }

    /**
     * Get the option 1 or 2 from the user (ensure that the input is a 1 or 2 and not a string or other type of input)
     * @return 1 or 2
     */
    private int opcio1o2(){
        int opcio = -1;
        while (opcio != 1 && opcio != 2) {
            try {
                opcio = sc.nextInt();
                sc.nextLine(); // Limpiar el buffer
                if (opcio != 1 && opcio != 2) {
                    System.out.println("Opció incorrecta. Torna a intentar (1 o 2).");
                }
            } catch (java.util.InputMismatchException e) {
                System.out.println("Entrada no vàlida. Introdueix 1 o 2.");
                sc.next();  // avoid infinite loop
            }
        }
        return opcio;
    }


    /**
     * Get the option byte from the user (ensure that the input is a byte and not a string or other type of input)
     * @return choice of user as a byte
     */
    private byte opcioByte(){
        byte opcio = -1;
        while (opcio < 0) {
            try {
                opcio = sc.nextByte();
                if (opcio < 0) {
                    System.out.println("Opció incorrecta. Torna a intentar.");
                }
            } catch (java.util.InputMismatchException e) {
                System.out.println("Entrada no vàlida. Introdueix un número enter positiu.");
                sc.next();  // avoid infinite loop
            }
        }
        return opcio;
    }

    /**
     * Routine to leave the game and close the connection (Leave + ok)
     * @throws IOException If there is an error in communication with the server
     * @throws GameException If there is an error in the game
     */
    private void leaveRoutine() throws IOException, GameException{

        System.out.println("Sortint...");
        bsComUtils.write_LEAVE(playerId, gameId);

        MessageAbstract message = bsComUtils.read();

        if (message instanceof MessageError) {
            MessageError error = (MessageError) message;
            System.out.println("Error received from server while leaving : " + error.getErrorDescription());
        } else if (message instanceof MessageOK) {
            System.out.println("Leaving succesful");
        } else {
            System.out.println("UNEXPECTED ERROR (leaveRoutine) : Unexpected message type --> " + message.getMessageType());
        }
        closeConnection();
        System.exit(0);
    }

    /**
     * Send a GETCONFIG message to the server and read the response
     * ONLY SENT ONCE, IF WE SEND ANOTHER GET CONFIG WE WOULD LOSE THE REMAINING SHIPS TO ADD!
     * @throws IOException If there is an error in communication with the server
     */
    private void sendGetConfig() throws IOException{
        bsComUtils.write_GETCONFIG(playerId, gameId);
        MessageAbstract message = bsComUtils.read();

        if (message instanceof MessageError) {
            // if we receive an error, we have to close the connection (ids are not correct)
            MessageError error = (MessageError) message;
            System.out.println("Error received from server : " + error.getErrorDescription());
            closeConnection();
            System.exit(1);

        } else if (message instanceof MessageGameConfig) {
            MessageGameConfig config = (MessageGameConfig) message;
            gameInfo.setWandH(config.getW(), config.getH());
            gameInfo.nships1 = config.getT1();
            gameInfo.nships2 = config.getT2();
            gameInfo.nships3 = config.getT3();
            gameInfo.nships4 = config.getT4();
            gameInfo.nships5 = config.getT5();

        } else {
            System.out.println("UNEXPECTED ERROR (sendGetConfig) : Unexpected message type --> " + message.getMessageType());
            closeConnection();
            System.exit(1);
        }
    }
}

