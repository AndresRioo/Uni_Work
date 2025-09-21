import axios from "axios";

class AuthService {
  login(user) {
    return axios.post("/api/token/", {
      username: user.username,
      password: user.password,
    });
  }

  register(user) {
  return axios.post("/api/v1/user/", {
    username: user.username,
    password: user.password,
    email: user.email,
  });
}


  refresh(refreshToken) {
    
    return axios.post("/api/token/refresh/", {
      refresh: refreshToken,
    });

    return Promise.resolve(
      JSON.stringify({
        access: "mockAccessToken",
      })
    );
  }

  logout() {
    sessionStorage.removeItem("access");
    sessionStorage.removeItem("refresh");
  }

  getAccessToken() {
    return sessionStorage.getItem("access");
  }
  getAllPlayers() {
    return this.getAxiosInstance().get("/api/v1/players/");
    return this.getAxiosInstance().get("/api/v1/players/", {
      headers: {
        Authorization: `Bearer ${token}`,
      },
    });
  }

  getAllGames() {
    return this.getAxiosInstance().get("/api/v1/games/");
    return this.getAxiosInstance().get("/api/v1/games/", {
      headers: {
        Authorization: `Bearer ${token}`,
      },
    });
  }


  getRefreshToken() {
    return sessionStorage.getItem("refresh");
  }

  isLoggedIn() {
    return !!sessionStorage.getItem("access");
  }

  getAllUserStats() {
    return this.getAxiosInstance().get(`/api/v1/user/`);
  }


  getAxiosInstance() {
    const apiUrl = import.meta.env.VITE_API_URL;
    const instance = axios.create({
      baseURL: apiUrl,
      headers: {
        Authorization: `Bearer ${this.getAccessToken()}`,
      },
    });

    instance.interceptors.response.use(
      (response) => response,
      async (error) => {
        if (error.response.status === 401 && this.isLoggedIn()) {
          try {
            const response = await this.refresh(this.getRefreshToken());
            sessionStorage.setItem("access", response.data.access);
            error.config.headers["Authorization"] =
              "Bearer " + response.data.access;
            return axios.request(error.config);
          } catch (err) {
            this.logout();
          }
        }
        return Promise.reject(error);
      }
    );

    return instance;
  }
}


export default new AuthService();
