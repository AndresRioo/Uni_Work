<script setup>
import { ref, onMounted } from "vue";
import { useAuthStore } from "../store/authStore";

const authStore = useAuthStore();

const username = ref("");
const password = ref("");

const registerUsername = ref("");
const registerEmail = ref("");
const registerPassword = ref("");
const registerConfirmPassword = ref("");

const successMessage = ref("");
const registerError = ref("");


onMounted(() => {
  authStore.initializeAuthStore();
  authStore.getAllPlayers(); // <- actualitza l'estat de playersList al principi
  authStore.getWaitListGames();
  authStore.getListGames();
  authStore.getTopPlayers(); // Obtenim els 5 millors jugadors
});

const startGame = () => {
  window.location.href = "/game";
};

const validatePassword = (password) => {
  const minLength = 8;
  const hasUpperCase = /[A-Z]/.test(password);
  const hasNumber = /\d/.test(password);
  const hasSpecialChar = /[!@#$%^&*(),.?":{}|<>]/.test(password);

  if (password.length < minLength) return "La contraseña debe tener al menos 8 caracteres";
  if (!hasUpperCase) return "La contraseña debe tener al menos una letra mayúscula";
  if (!hasNumber) return "La contraseña debe tener al menos un número";
  if (!hasSpecialChar) return "La contraseña debe tener al menos un carácter especial";

  return null; // Si pasa todo
};

const authenticateUser = () => {
  if (!username.value || !password.value) {
    alert("Please enter both username and password.");
    return;
  }
  authStore.login({ username: username.value, password: password.value });

};

const logOut = () => {
  authStore.logout();
};


const goToGame = (gameId) => {
  sessionStorage.setItem("currentGameId", gameId);
  window.location.href = "/game";
};



const registerUser = () => {
  registerError.value = "";

  if (
    !registerUsername.value.trim() ||
    !registerEmail.value.trim() ||
    !registerPassword.value.trim() ||
    !registerConfirmPassword.value.trim()
  ) {
    registerError.value = "Tots els camps són obligatoris.";
    return;
  }

  const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  if (!emailRegex.test(registerEmail.value)) {
    registerError.value = "El correu electrònic no és vàlid.";
    return;
  }

  const pwdError = validatePassword(registerPassword.value);
  if (pwdError) {
    registerError.value = pwdError;
    return;
  }

  if (registerPassword.value !== registerConfirmPassword.value) {
    registerError.value = "Les contrasenyes no coincideixen.";
    return;
  }
  
  // trucar al Registre amb els valors de la vista
  authStore.register({
    username: registerUsername.value,
    email: registerEmail.value,
    password: registerPassword.value,
  })
  .then(() => {
    // neteja de camps
    registerUsername.value = "";
    registerEmail.value = "";
    registerPassword.value = "";
    registerConfirmPassword.value = "";

    // tot ha anat bé
    successMessage.value = "Usuario registrado correctamente";
    registerError.value = "";
  })
  .catch((error) => {
    // no ha anat bé
    registerError.value =
      authStore.error || error.response?.data?.detail || "Error al registrar usuario";
    successMessage.value = "";
  });



};


</script>

<template>
  <div class="home text-center mt-4">
    <h1>Welcome to Battleship Game</h1>
    <div v-if="authStore.isAuthenticated" class="mt-5">
      <h3>You're logged in!</h3>
      <div class="mb-3">
        Access Token: {{ authStore.accessToken.slice(0, 20) }}...
      </div>
      <button class="btn btn-primary mr-2" @click="startGame">
        Start New Game
      </button>
      <button class="btn btn-secondary" @click="logOut">Log Out</button>
    </div>

    <div v-else class="d-flex justify-content-around mt-4 flex-wrap">
      <!-- Login -->
      <div style="max-width: 300px">
        <h3>Inici de sessió</h3>
        <form @submit.prevent="authenticateUser">
          <input
            v-model="username"
            type="text"
            placeholder="Nom d'usuari"
            class="form-control mb-2"
          />
          <input
            v-model="password"
            type="password"
            placeholder="Contrasenya"
            class="form-control mb-2"
          />
          <button class="btn btn-primary w-100" :disabled="authStore.loading">
            {{ authStore.loading ? "Iniciant..." : "Inicia sessió" }}
          </button>
          <div v-if="authStore.error" class="text-danger mt-2">
            {{ authStore.error }}
          </div>
        </form>
      </div>

      <!-- Register -->
      <div style="max-width: 300px">
        <h3>Registre</h3>
        <form @submit.prevent="registerUser">
          <input
            v-model="registerUsername"
            type="text"
            placeholder="Nom d'usuari"
            class="form-control mb-2"
          />
          <input
            v-model="registerEmail"
            type="email"
            placeholder="Correu electrònic"
            class="form-control mb-2"
          />
          <input
            v-model="registerPassword"
            type="password"
            placeholder="Contrasenya"
            class="form-control mb-2"
          />
          <input
            v-model="registerConfirmPassword"
            type="password"
            placeholder="Confirmació de la contrasenya"
            class="form-control mb-2"
          />
          <small class="text-muted">
            La contrasenya ha de tenir almenys 8 caràcters, una majúscula, una minúscula i un número.
          </small>
          <button class="btn btn-success w-100">Registrar-se</button>
          <div v-if="registerError" class="text-danger mt-2">
            {{ registerError }}
          </div>

          <div v-if="successMessage" class="text-success mt-2">
            {{ successMessage }}
          </div>


        </form>
      </div>
    </div>

    <div
        v-if="authStore.isAuthenticated"
        class="d-flex justify-content-start flex-row align-items-stretch"
        style="margin-top: 20px; gap: 20px;"
    >
      <!-- Llista de jugadors -->
      <div
          v-if="authStore.playersList.length > 0"
          class="d-flex flex-column"
          style="min-width: 30%; text-align: center;"
      >
        <h3>Jugadors existents</h3>
        <ul class="list-group flex-grow-1">
          <li
              v-for="player in authStore.playersList"
              :key="player.id"
              class="list-group-item"
          >
            {{ player.nickname }}
          </li>
        </ul>
      </div>
      <!-- Llista de partidas -->
      <div class="d-flex flex-column" style="min-width: 30%;">
        <h3 class="mb-3">Partides existents</h3>
        <ul class="list-group flex-grow-1">
          <li
              v-if="authStore.listGames.length === 0"
              class="list-group-item text-muted"
          >
            No hi ha partides disponibles.
          </li>
          <li
              v-for="game in authStore.listGames"
              :key="game.id"
              class="list-group-item d-flex flex-column list-item-hover"
              @click="goToGame(game.id)"
          >
            <div><strong>Partida ID:</strong> {{ game.id }}</div>
            <div><strong>Fase:</strong> {{ game.phase }}</div>
            <div><strong>Jugador 1:</strong> {{ game.player1 }}</div>
            <div><strong>Jugador 2:</strong> {{ (game.player2 === 'bot' || !game.player2)? authStore.username : game.player2 }}</div>
          </li>
        </ul>
      </div>
      <!-- Llista dels 5 millors jugadors -->
      <div
          v-if="authStore.topPlayers.length > 0"
          class="d-flex flex-column"
          style="min-width: 30%; text-align: center;"
      >
        <h3>Top 5 Jugadors</h3>
        <ul class="list-group flex-grow-1">
          <li
              v-for="player in authStore.topPlayers"
              :key="player.id"
              class="list-group-item d-flex justify-content-between"
          >
            <span>{{ player.nickname }}</span>
            <span>{{ player.score }}%</span>
          </li>
        </ul>
      </div>
    </div>
  </div>
</template>


<style scoped>
.home {
  max-width: 600px;
  margin: 0 auto;
}
.list-item-hover {
  cursor: pointer;
  transition: background-color 0.2s ease;
}
.list-item-hover:hover {
  background-color: #f0f0f0;
}

</style>
