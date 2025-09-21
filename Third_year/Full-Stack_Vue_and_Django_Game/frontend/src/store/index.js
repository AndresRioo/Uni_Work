import { defineStore } from "pinia";
import api from "../services/api";

export const useGameStore = defineStore("game", {
  state: () => ({
    gamePhase: "placement",
    gameStatus: "Place your ships",
    playerBoard: [],
    opponentBoard: [],
    playerPlacedShips: [],
    opponentShips: [],
    availableShips: [],
    selectedShip: null,
  }),

  actions: {
    getUser(id) {
      return api
        .getUser(id)
        .then((response) => {
          return response.data;
        })
        .catch((error) => {
          const message = error.response?.data?.detail || error.message;
          throw new Error(message);
        });
    },

    async getGameState(gameId) {
      return api
        .getGameState(gameId)
        .then((response) => {
          // response = JSON.parse(response); // this is a mock, in real case it will be axios response


          const gameId = sessionStorage.getItem("currentGameId"); // guardar la info por si se recarga la partida

          console.log("GameID DE SESSION STORAGE --> ", gameId);
          console.log("this.availableShips --> " , this.availableShips)


          console.log("RESPONSE GET GAME STATE", response);

          console.log(" [GetGameState] response.data", response.data);

          const gameState = response.data.gameState.game_state_response.data.gameState;

          console.log(" [GetGameState] gameState", gameState);

          if (gameState.player1.username == "bot") {
            this.playerBotId = gameState.player1.id;
            this.player2Id = gameState.player2.id; // el jugador humano
            this.playerBotUsername = gameState.player1.username;
            this.player2Username = gameState.player2.username;

            this.playerBoard = gameState.player2.board;
            this.opponentBoard = gameState.player1.board;
            this.playerPlacedShips = gameState.player2.placedShips;
            this.opponentShips = gameState.player1.placedShips;

          } else {
            this.playerBotId = gameState.player2.id;
            this.player2Id = gameState.player1.id;
            this.playerBotUsername = gameState.player2.username;
            this.player2Username = gameState.player1.username;

            this.playerBoard = gameState.player1.board;
            this.opponentBoard = gameState.player2.board;
            this.playerPlacedShips = gameState.player1.placedShips;
            this.opponentShips = gameState.player2.placedShips;
          }


          console.log("Player Bot ID:", this.playerBotId);
          console.log("Player Bot username:", this.playerBotUsername);

          console.log("Player 2 ID:", this.player2Id);
          console.log("Player 2 username:", this.player2Username);

          
          //this.availableShips = gameState.player1.availableShips;
          this.gamePhase = gameState.phase;
          this.gameId = gameState.gameId;
          this.currentGameId = gameState.gameId;
          // this.gameStatus =
          //   gameState.turn === "player1" ? "Your turn" : "Opponent's turn";
          if (this.gamePhase === "playing") {
            this.gameStatus =
              gameState.turn === this.player2Username ? "Your turn" : "Opponent's turn";
              console.log("TURNO DE --> ",gameState.turn)
          } else if (this.gamePhase === "placement" || this.gamePhase === "waiting" ) {

            this.gamePhase = "waiting";
            this.gameStatus = "Place your ships";

          } else if (this.gamePhase === "gameOver") {
            this.gameStatus = "Game Over - Winner " + gameState.winner;
          }
        })
        .catch((error) => {
          const message = error.response?.data?.detail || error.message;
          throw new Error(message);
        });
    },
    createEmptyBoard() {
      return Array(10)
        .fill()
        .map(() => Array(10).fill(0));
    },

    async startNewGameFromPlayingGame() {

      try{  
      this.gamePhase = "placement";
      this.gameStatus = "Place your ships";
      this.playerBoard = this.createEmptyBoard();
      this.opponentBoard = this.createEmptyBoard();
      this.playerPlacedShips = [];
      this.opponentShips = [];
      this.selectedShip = null;
      
      // Espera los barcos disponibles
      this.availableShips = await api.getAvailableShips();

      // Espera a que se cree la partida
      const response = await api.createGame();

      console.log("RESPONSE DE CREATE GAME : ", response);

      const newGame = response.data;
      this.currentGameId = newGame.id; // Guardamos el ID por si se necesita luego        

      console.log("NEW GAME : ", newGame);
      console.log("currentGameId :  ", this.currentGameId);

      const state = newGame.game_state_response.data.gameState;

      console.log("state : ", state);
      console.log("username player 1", state.player1.username)
      // Detecta cuál es el bot y cuál es el humano
      if (state.player1.username == "bot") {
        this.playerBotId = state.player1.id;
        this.player2Id = state.player2.id; // el jugador humano
        this.playerBotUsername = state.player1.username;
        this.player2Username = state.player2.username;

      } else {
        this.playerBotId = state.player2.id;
        this.player2Id = state.player1.id;
        this.playerBotUsername = state.player2.username;
        this.player2Username = state.player1.username;
      }


      console.log("Player Bot ID:", this.playerBotId);
      console.log("Player Bot username:", this.playerBotUsername);

      console.log("Player 2 ID:", this.player2Id);
      console.log("Player 2 username:", this.player2Username);


      // Colocar los barcos del oponente 
      this.placeOpponentShips();

      await api.addVesselsToPlayer(this.currentGameId, this.playerBotId ,this.opponentShips);

      } catch (error) {
        const message = error.response?.data?.detail || error.message || "Error inesperado.";
        this.gameStatus = `Error al crear la partida: ${message}`;
        console.error("Error en startNewGame:", error);
      }

    },


    async startNewGame() {

      console.log("StartNewGame --> currentGameId :  ", sessionStorage.getItem("currentGameId"));
      console.log("this.availableShips --> " , this.availableShips);
      console.log("sessionStorage es -1?", sessionStorage.getItem("currentGameId") == "-1" );
      console.log("el if 1",sessionStorage.getItem("currentGameId") != null)
      console.log("el if 2",sessionStorage.getItem("currentGameId") != "-1")

      console.log("this.gamePhase",this.gamePhase);
      console.log(this.gamePhase != "waiting");
      console.log(sessionStorage.getItem("currentGameId") != null && this.gamePhase != "waiting");
      console.log(sessionStorage.getItem("currentGameId") != null && this.gamePhase != "waiting" && this.gamePhase != "GameOver");

      // si ya hay un id guardado, no estamos poniendo barcos y no se ha acabado, entonces cogemos la partida guardada de la bd   
      if (sessionStorage.getItem("currentGameId") != null && this.gamePhase != "waiting" && this.gamePhase != "GameOver"){
          await this.getGameState(sessionStorage.getItem("currentGameId")); // <-- LLAMADA AL GET game state para evitar problemas con el refresh
          return;
      } else {
          console.log("StartNewGame NULO");
      }

    try{  
      this.gamePhase = "placement";
      this.gameStatus = "Place your ships";
      this.playerBoard = this.createEmptyBoard();
      this.opponentBoard = this.createEmptyBoard();
      this.playerPlacedShips = [];
      this.opponentShips = [];
      this.selectedShip = null;
      
      // Espera los barcos disponibles
      this.availableShips = await api.getAvailableShips();

      // Espera a que se cree la partida
      const response = await api.createGame();

      console.log("RESPONSE DE CREATE GAME : ", response);

      const newGame = response.data;
      this.currentGameId = newGame.id; // Guardamos el ID por si se necesita luego        

      console.log("NEW GAME : ", newGame);
      console.log("currentGameId :  ", this.currentGameId);

      const state = newGame.game_state_response.data.gameState;

      console.log("state : ", state);
      console.log("username player 1", state.player1.username)
      // Detecta cuál es el bot y cuál es el humano
      if (state.player1.username == "bot") {
        this.playerBotId = state.player1.id;
        this.player2Id = state.player2.id; // el jugador humano
        this.playerBotUsername = state.player1.username;
        this.player2Username = state.player2.username;

      } else {
        this.playerBotId = state.player2.id;
        this.player2Id = state.player1.id;
        this.playerBotUsername = state.player2.username;
        this.player2Username = state.player1.username;
      }


      console.log("Player Bot ID:", this.playerBotId);
      console.log("Player Bot username:", this.playerBotUsername);

      console.log("Player 2 ID:", this.player2Id);
      console.log("Player 2 username:", this.player2Username);


      // Colocar los barcos del oponente 
      this.placeOpponentShips();

      await api.addVesselsToPlayer(this.currentGameId, this.playerBotId ,this.opponentShips);

      } catch (error) {
        const message = error.response?.data?.detail || error.message || "Error inesperado.";
        this.gameStatus = `Error al crear la partida: ${message}`;
        console.error("Error en startNewGame:", error);
      }
    },


    placeShip(board, row, col, size, isVertical, type) {
      for (let i = 0; i < size; i++) {
        const r = isVertical ? row + i : row;
        const c = isVertical ? col : col - i;
        board[r][c] = type;
      }
    },

    isValidPlacement(board, row, col, size, isVertical) {
      const inBounds = isVertical ? row + size <= 10 : col + 1 - size >= 0;
      if (!inBounds) return false;

      for (let i = 0; i < size; i++) {
        const r = isVertical ? row + i : row;
        const c = isVertical ? col : col - i;
        if (board[r][c] !== 0) return false;
      }
      return true;
    },

    placeOpponentShips() {
      const shipList = [1, 2, 3, 4, 5];
      for (let type of shipList) {
        const ship = this.availableShips.find((s) => s.type === type);
        let placed = false;
        while (!placed) {
          const row = Math.floor(Math.random() * 10);
          const col = Math.floor(Math.random() * 10);
          const isVertical = Math.random() > 0.5;
          if (
            this.isValidPlacement(
              this.opponentBoard,
              row,
              col,
              ship.size,
              isVertical
            )
          ) {
            this.placeShip(
              this.opponentBoard,
              row,
              col,
              ship.size,
              isVertical,
              ship.type
            );
            this.opponentShips.push({
              ...ship,
              isVertical,
              position: { row, col },
            });
            placed = true;
          }
        }
      }

    },

    selectShip(ship) {
      this.selectedShip = { ...ship };
    },

    rotateSelectedShip() {
      if (this.selectedShip) {
        this.selectedShip.isVertical = !this.selectedShip.isVertical;
      }
    },

    async handlePlayerBoardClick(row, col) {
      if (this.gamePhase !== "placement" || !this.selectedShip) return;
      const ship = this.selectedShip;
      console.log("SHIP -->",ship);
      if (
        !this.isValidPlacement(
          this.playerBoard,
          row,
          col,
          ship.size,
          ship.isVertical
        )
      )
        return;

      this.placeShip(
        this.playerBoard,
        row,
        col,
        ship.size,
        ship.isVertical,
        ship.type
      );

      this.playerPlacedShips.push({ ...ship, position: { row, col } });

      this.availableShips = this.availableShips.filter(
        (s) => s.type !== ship.type
      );
      this.selectedShip = null;

      if (this.availableShips.length === 0) {
        this.gamePhase = "playing";
        this.gameStatus = "Your turn";
              
        await api.addVesselsToPlayer(this.currentGameId, this.player2Id , this.playerPlacedShips);
        await api.changeStateToPlaying(this.currentGameId);

        sessionStorage.setItem("currentGameId", this.currentGameId); // guardar la info por si se recarga la partida


      }
    },

    async handleOpponentBoardClick(row, col) {
      if (this.gamePhase !== "playing") return;
      if (this.opponentBoard[row][col] < 0) {
        this.gameStatus = "Already hit!";
        return;
      } else if (this.opponentBoard[row][col] === 11) {
        this.gameStatus = "Already missed!";
        return;
      }

        try {
          const response = await api.checkHitPlayer(this.currentGameId, this.player2Id , row, col);
          const data = response.data;
          console.log("Respuesta recibida:", data);

          
        if (data.result === 1 || data.result === 2) {
          // 1 = hit, 2 = hundido
          console.log(`Hit en columna ${col}, fila ${row}`);
          this.opponentBoard[row][col] = -1; // marcar como hit
          this.gameStatus = data.result === 2 ? "Ship Sunk!" : "Hit!";
        } else if (data.result === 0) {
          console.log("Has fallado.");
          this.opponentBoard[row][col] = 11; // marcar como miss
          this.gameStatus = "Miss!";
        }

        if (data.result == 3) { // codigo para acabar partida (viene dado por un sunk)
          console.log(`Hit en columna ${col}, fila ${row}`);
          this.opponentBoard[row][col] = -1; // marcar como hit
          this.gameStatus = "You win!";
          this.gamePhase = "gameOver";

          console.log("HAS GANADO ! El Jugador -> ", this.player2Username);

          api.sumarVictoria(this.player2Id); // para la estadistica

          sessionStorage.removeItem("currentGameId");  // para terminar la partida

           // Esperar 3 segundos antes de volver al menú
            setTimeout(() => {
              window.location.href = "/";
            }, 5000);

          return; // Para evitar que setTimeout dispare el turno del bot
        }


        } catch (error) {
          console.error("Error en la petición:", error);
        }

      // por si gana el bot
      setTimeout(() => {
        if (this.gamePhase === "playing") {
          this.opponentTurn();
        }
      }, 500);

    
    },

    async opponentTurn() {
      let row,
        col,
        valid = false;
      while (!valid) {
        row = Math.floor(Math.random() * 10);
        col = Math.floor(Math.random() * 10);
        valid =
          this.playerBoard[row][col] >= 0 && this.playerBoard[row][col] < 10;
      }

      // const isHit =
      //   this.playerBoard[row][col] > 0 && this.playerBoard[row][col] < 10;
      // this.playerBoard[row][col] = isHit ? -this.playerBoard[row][col] : 11;

      try {
          const response = await api.checkHitBot(this.currentGameId, this.playerBotId, row, col);
          const data = response.data;
          console.log("Respuesta recibida:", data);
          // usar data aquí

          
        if (data.result === 1 || data.result === 2) {
          // 1 = hit, 2 = hundido
          console.log(`Hit en columna ${col}, fila ${row}`);
          this.playerBoard[row][col] = -1; // marcar como hit
          this.gameStatus = data.result === 2 ? "Ship Sunk!" : "Hit!";
        } else if (data.result === 0) {
          console.log("Bot fallado.");
          this.playerBoard[row][col] = 11; // marcar como miss
          this.gameStatus = "Miss!";
        }

        if (data.result === 3) { // codigo de final de partida 
            console.log(`Hit en columna ${col}, fila ${row}`);
            this.playerBoard[row][col] = -1; // marcar como hit
            this.gameStatus = "You Lose!";
            this.gamePhase = "gameOver";

            console.log("Fin de la partida, user ha perido")

            api.sumarPartida(this.playerBotId); // para la estadisticac

            sessionStorage.removeItem("currentGameId"); 

           // Esperar 3 segundos antes de volver al menú
            setTimeout(() => {
              window.location.href = "/";
            }, 3000);

            return; 
        }

        } catch (error) {
          console.error("Error en la petición:", error);
        }


      this.gameStatus = "Your turn";
    },
  },
});
