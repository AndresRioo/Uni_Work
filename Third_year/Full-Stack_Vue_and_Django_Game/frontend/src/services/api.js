import AuthService from "@/services/auth.js";
const axiosInstance = AuthService.getAxiosInstance();

export default {
  getAvailableShips() {
    return Promise.resolve([
      {
        type: 1,
        isVertical: true,
        size: 1,
      },
      {
        type: 2,
        isVertical: true,
        size: 2,
      },
      {
        type: 3,
        isVertical: true,
        size: 3,
      },
      {
        type: 4,
        isVertical: true,
        size: 4,
      },
      {
        type: 5,
        isVertical: true,
        size: 5,
      },
    ]);
  },

  getGameState(gameId) {

    // SESSION 3  : cambiar el promise por una llamada real al backend
    //return axiosInstance.get(`/api/v1/games/${gameId}/`);

    return axiosInstance.get(`/api/v1/games/${gameId}/`)
    .then(response => {
      return {
        status: response.status,
        message: "OK",
        data: {
          gameState: response.data
        }
      };
    });

    return Promise.resolve(
      JSON.stringify({
        status: 200,
        message: "OK",
        data: {
          gameState: {
            gameId: "12345",
            phase: "playing", // "placement", "playing", "gameOver"
            turn: "player1",
            winner: null,
            player1: {
              id: "1",
              username: "admin",
              placedShips: [
                {
                  type: 1,
                  position: { row: 1, col: 3 },
                  isVertical: true,
                  size: 1,
                },
                {
                  type: 2,
                  position: { row: 3, col: 4 },
                  isVertical: false,
                  size: 2,
                },
                {
                  type: 3,
                  position: { row: 5, col: 2 },
                  isVertical: true,
                  size: 3,
                },
                {
                  type: 4,
                  position: { row: 6, col: 7 },
                  isVertical: false,
                  size: 4,
                },
                {
                  type: 5,
                  position: { row: 1, col: 8 },
                  isVertical: true,
                  size: 5,
                },
              ],
              availableShips: [],
              board: [
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 11, 0, 1, 0, 0, 0, 0, 5, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 5, 0],
                [0, 0, 0, 2, 2, 0, 0, 0, 5, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 5, 0],
                [0, 0, -3, 0, 0, 0, 0, 0, 5, 0],
                [0, 0, 3, 0, 4, -4, 4, 4, 0, 0],
                [0, 0, 3, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 11, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
              ],
            },
            player2: {
              id: "2",
              username: "player2",
              placedShips: [
                {
                  type: 1,
                  position: { row: 1, col: 3 },
                  isVertical: true,
                  size: 1,
                },
                {
                  type: 2,
                  position: { row: 3, col: 4 },
                  isVertical: false,
                  size: 2,
                },
                {
                  type: 3,
                  position: { row: 5, col: 2 },
                  isVertical: true,
                  size: 3,
                },
                {
                  type: 4,
                  position: { row: 6, col: 7 },
                  isVertical: false,
                  size: 4,
                },
                {
                  type: 5,
                  position: { row: 1, col: 8 },
                  isVertical: true,
                  size: 5,
                },
              ],
              availableShips: [],
              board: [
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 11, 0, 1, 0, 0, 0, 0, 5, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 5, 0],
                [0, 0, 0, 2, 2, 0, 0, 0, 5, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 5, 0],
                [0, 0, -3, 0, 0, 0, 0, 0, 5, 0],
                [0, 0, 3, 0, 4, -4, 4, 4, 0, 0],
                [0, 0, 3, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 11, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
              ],
            },
          },
        },
      })
    );
  },

  getUser(id) {
    return axiosInstance.get(`/api/v1/user/${id}`);
  },

  createGame() {
    return axiosInstance.post(`/api/v1/games/`);
  },

  addPlayerToGame(gameId) {
    return axiosInstance.post(`/api/v1/games/${gameId}/players/`);
  },

  addVesselsToPlayer(gameId, playerId, vessels) {
      const requests = vessels.map(vessel => {
        const payload = {
          type: vessel.type,
          position: { ...vessel.position },
          isVertical: vessel.isVertical,
          size: vessel.size,
        };

        console.log("Payload enviado:", payload);

        return axiosInstance.post(`/api/v1/games/${gameId}/players/${playerId}/vessels/`, payload);
      });

      return Promise.all(requests);
    },

changeStateToPlaying(gameId) {
  const payload = {
    phase: "playing",
    turn: "player2", // O "player1", según quién empiece
  };

  console.log("Payload enviado:", payload);

  return axiosInstance.patch(`/api/v1/games/${gameId}/`, payload);
},

  // Dispara el jugador
  checkHitPlayer(gameId, playerId ,row,col){

      const payload = {
        row: row,
        col: col 
      }

      console.log("Payload enviado:", payload);

      console.log("Id del player en el shot : ", playerId)

      return axiosInstance.post(`/api/v1/games/${gameId}/players/${playerId}/shots/`, payload);
  },

  // Dispara el bot
  checkHitBot(gameId,botId,  row,col){

      const payload = {
        row: row,
        col: col 
      }

      console.log("Payload enviado:", payload);

      console.log("Id del bot en el shot : ", botId)

      return axiosInstance.post(`/api/v1/games/${gameId}/players/${botId}/shots/`, payload);

  },

    // Para la top list
    sumarPartida(player2Id) {
        return axiosInstance.patch(`/api/v1/user/${player2Id}/`, { action: "partida" });
    },

    // Para la top list
    sumarVictoria(player2Id) {
        return axiosInstance.patch(`/api/v1/user/${player2Id}/`, { action: "victoria" });
    }








};
