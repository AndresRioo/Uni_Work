import { defineStore } from "pinia";
import AuthService from "../services/auth";

export const useAuthStore = defineStore("auth", {
  state: () => ({
    username: null,
    accessToken: null,
    refreshToken: null,
    isAuthenticated: false,
    loading: false,
    error: null,
    playersList: [],
    waitListGames: [],
    listGames: [],
    topPlayers: [],
  }),
  actions: {
    initializeAuthStore() {
      this.username = sessionStorage.getItem("username");
      this.accessToken = sessionStorage.getItem("access");
      this.refreshToken = sessionStorage.getItem("refresh");
      this.isAuthenticated = !!this.accessToken;
    },
    async login(user) {

      this.loading = true;
      this.error = null;

      try {
        // 1. Login: obtener tokens
        const response = await AuthService.login(user);
        this.username = user.username;
        this.accessToken = response.data.access;
        this.refreshToken = response.data.refresh;
        this.isAuthenticated = true;

        sessionStorage.setItem("username", this.username);
        sessionStorage.setItem("access", this.accessToken);
        sessionStorage.setItem("refresh", this.refreshToken);

        // 2. Obtener información del usuario (incluye ID)
        const userInfo = await AuthService.getAxiosInstance().get("/api/v1/user/me/", {
          headers: {
            Authorization: `Bearer ${this.accessToken}`,
          },
        });

        this.userId = userInfo.data.id;
        sessionStorage.setItem("userId", this.userId);

        // 3. Obtener cosas necesarias tras login
        this.getAllPlayers();
        this.getListGames();
        this.getWaitListGames();
        this.getTopPlayers();
      } catch (error) {
        console.log("error", error);
        this.error = error.response?.data?.detail || "Login failed. Try again.";
        this.isAuthenticated = false;
      } finally {
        this.loading = false;
      }
    },

    refreshToken() {
      const refresh = sessionStorage.getItem("refresh");
      if (!refresh) return;

      this.loading = true;

      AuthService.refresh(refresh)
        .then((response) => {
          this.accessToken = response.data.access;
          sessionStorage.setItem("access", this.accessToken);
        })
        .catch((error) => {
          console.error("Error al refrescar token:", error);
          this.logout();
        })
        .finally(() => {
          this.loading = false;
        });
    },


    logout() {
      this.accessToken = null;
      this.refreshToken = null;
      this.isAuthenticated = false;
      sessionStorage.removeItem("username");
      sessionStorage.removeItem("access");
      sessionStorage.removeItem("refresh");
      sessionStorage.removeItem("currentGameId");
      sessionStorage.removeItem("userId");

    },

    register(user) {
      this.loading = true;
      this.error = null;

      return AuthService.register(user)
        .then((response) => {
          return response; // devolvemos la respuesta al componente de vue
        })
        .catch((error) => {
          const data = error.response?.data;
          if (data) {
            const firstKey = Object.keys(data)[0];
            this.error = data[firstKey][0];
          } else {
            this.error = "Error en el registro. Inténtalo de nuevo.";
          }
          throw error; // lanzamos error para que el componente lo maneje
        })
        .finally(() => {
          this.loading = false;
        });
    },
      async getListGames() {
          try {
              const response = await AuthService.getAllGames();
              this.listGames = [];
              for (const game of response.data) {
                  this.listGames.push({
                      id: game.id,
                      phase: game.phase,
                      player1: game.game_state_response.data.gameState.player1.username,
                      player2: game.game_state_response.data.gameState.player2.username,
                  });
              }
          } catch (error) {
              const message = error.response?.data?.detail || error.message;
              throw new Error(message);
          }
      },
      async getWaitListGames() {
          try {
              const response = await AuthService.getAllGames();
              this.waitListGames = [];
              for (const game of response.data) {
                  if (game.game_state_response.data.gameState.player1 === null || game.game_state_response.data.gameState.player2 === null) {
                      this.waitListGames.push({
                          id: game.id,
                          phase: game.phase,
                          player1: game.game_state_response.data.gameState.player1.username,
                          player2: game.game_state_response.data.gameState.player2.username,
                      });
                  }
              }
          } catch (error) {
              const message = error.response?.data?.detail || error.message;
              throw new Error(message);
          }
      },

      async getTopPlayers() {
          try {
            const response = await AuthService.getAllUserStats(); // usuarios con partidas_jugadas y partidas_ganadas

            this.topPlayers = response.data.map(user => {
              const totalGames = user.partidas_jugadas || 0;
              const winnedGames = user.partidas_ganadas || 0;
              const score = totalGames > 0 ? (winnedGames / totalGames) * 100 : 0;

              return {
                id: user.id,
                nickname: user.nickname || user.username,
                score: score.toFixed(2),
              };
            });

            this.topPlayers.sort((a, b) => b.score - a.score);
            this.topPlayers = this.topPlayers.slice(0, 5);

          } catch (error) {
            const message = error.response?.data?.detail || error.message;
            throw new Error(message);
          }
        },

     
    

      async getAllPlayers() {
          try {
              const response = await AuthService.getAllPlayers();
              this.playersList = [];
              for (const player of response.data) {
                  this.playersList.push({
                      id: player.id,
                      nickname: player.nickname,
                  });
              }
          } catch (error) {
              const message = error.response?.data?.detail || error.message;
              throw new Error(message);
          }
      },


      async joinGame(gameId) {
        try {
          const response = await axios.post(`/api/v1/games/${gameId}/join/`, {}, {
            headers: {
              Authorization: `Bearer ${this.accessToken}`,
            }
          });

          console.log("Player Bot ID:", this.playerBotId);
          console.log("Player Bot username:", this.playerBotUsername);

          console.log("Player 2 ID:", this.player2Id);
          console.log("Player 2 username:", this.player2Username);

          return response.data;
        } catch (error) {
          console.error("Error en joinGame", error);
          throw error;
        }
      },

      
  },
});
