// Referencias al modal y botones
const exitModal = document.getElementById("exit-modal");
const exitYesButton = document.getElementById("exit-yes-button");
const exitNoButton = document.getElementById("exit-no-button");
const exitButton = document.querySelector(".exit-button");

// Mostrar el modal al hacer clic en el botón "X"
exitButton.addEventListener("click", () => {
    exitModal.style.display = "block";
});

// Redirigir a receptaMagdalenes.html si elige "Sí"
exitYesButton.addEventListener("click", () => {
    window.location.href = "receptaMagdalenes.html";
});

// Cerrar el modal si elige "No"
exitNoButton.addEventListener("click", () => {
    exitModal.style.display = "none";
});