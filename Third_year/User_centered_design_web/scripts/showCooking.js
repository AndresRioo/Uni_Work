// Array de pasos con texto e imágenes
const steps = [
    {
        id: 1,
        description: "Pas 1: Bat els ous amb unes varetes, amb moviments ràpids i envoltants.",
        images: ["../imatges/indicacions/batirHuevos.jpg"]
    },
    {
        id: 2,
        description: "Pas 2: Afegeix ara a poc a poc el sucre mentre bats amb moviments ràpids i envoltants la resta d'ingredients.",
        images: ["../imatges/indicacions/azucar.jpg", "../imatges/indicacions/huevosBatidos.jpg"]
    },
    {
        id: 3,
        description: "Pas 3: Coloca els motlles de silicona sobre la safata, i introdueix el paper de magdalenes dins de cada espai.",
        images: ["../imatges/indicacions/moldeMagdalenas.jpg", "../imatges/indicacions/papelMagdalenas.jpg"]
    },
    {
        id: 4,
        description: "Pas 4: Preescalfa el forn a 220 graus amb temperatura amunt i avall sense aire.",
        images: ["../imatges/indicacions/horno.jpg"]
    },
    {
        id: 5,
        description: "Pas 5: Omple fins a 3/4 els papers batent la massa perquè no s’espessi.",
        images: ["../imatges/indicacions/llenarMagdalenas.jpg"]
    },
    {
        id: 6,
        description: "Pas 6: Baixa la temperatura a 200 graus i enforna durant 15 minuts a la safata central.",
        images: ["../imatges/indicacions/horno.jpg"] // temporitzador
    },
    {
        id: 7,
        description: "Pas 7: Tritura els moniatos amb una forquilla fins a aconseguir un puré.",
        images: ["../imatges/indicacions/triturarBoniato.jpg"]
    },
    {
        id: 8,
        description: "Pas 8: Tritura el plàtan amb una forquilla i barreja-ho amb el puré de moniato i la crema de cacau fins obtenir una barreja homogènia.",
        images: ["../imatges/indicacions/triturarBoniato.jpg", "../imatges/indicacions/triturarPlatano.jpg", "../imatges/indicacions/cremaCacau.jpg"]
    },
    {
        id: 9,
        description: "Pas 9: Tamisa el cacau i la pols de coure, afegeix-los a la barreja i integra'ls bé.",
        images: ["../imatges/indicacions/tamizarCacau.jpg"]
    },
    {
        id: 10,
        description: "Pas 10: Afegeix-hi la farina d'ametlles i barreja-ho fins aconseguir una massa uniforme.",
        images: ["../imatges/indicacions/harinaMezclar.jpg"]
    },
    {
        id: 11,
        description: "Pas 11: Forra un motlle amb paper de coure i aboca la barreja",
        images: ["../imatges/indicacions/bandejaPapel.jpg", "../imatges/indicacions/browniesHorno.jpg"]
    },
    {
        id: 12,
        description: "Pas 12: Treu del forn les magdalenes i deixa-les repossar dins el motlle perquè assenti bé la massa.",
        images: ["../imatges/indicacions/horno.jpg", "../imatges/cupcakes_basic.jpg"]
    },
    {
        id: 13,
        description: "Pas 13: Forneja a 180 graus durant 25-30 minuts, calor amunt i avall, fins que en punxar amb un escuradents, surti net.",
        images: ["../imatges/indicacions/browniesHorno.jpg", "../imatges/indicacions/horno.jpg"]
    },
    {
        id: 14,
        description: "Pas 14: Treu els brownies del forn i deixa'ls repossar.",
        images: ["../imatges/indicacions/horno.jpg", "../imatges/brownies_chocolate.jpg"]
    }
];

// Variables para controlar el estado actual
let currentStepIndex = 0;

// Referencias a los elementos del DOM
const stepDescription = document.getElementById("step-description");
const stepImage = document.getElementById("step-image");
const prevButton = document.getElementById("prev-button");
const nextButton = document.getElementById("next-button");

// Referencias al temporizador
const timerContainer = document.getElementById("timer-container");
const timerDisplay = document.getElementById("timer");
const timerBtn = document.getElementById("timer-button");

let fakeInterval = null;
let fakeTimeLeft = 0;
let timerStarted = false;
let timerBtnEnabled = true;
let timerManuallyFinalized = false;

function resetTimer(seconds, showButton = true) {
    clearInterval(fakeInterval);
    fakeTimeLeft = seconds;
    timerStarted = false;
    timerBtn.textContent = "Empezar";
    timerDisplay.textContent = formatTime(fakeTimeLeft);
    timerBtn.style.display = showButton ? "inline-block" : "none";
}

function stopTimer(seconds, showButton = false) {
    clearInterval(fakeInterval);
    fakeTimeLeft = seconds;
    timerStarted = false;
    timerDisplay.textContent = formatTime(fakeTimeLeft);
    timerBtn.style.display = showButton ? "inline-block" : "none";
}

function formatTime(totalSeconds) {
    const minutes = Math.floor(totalSeconds / 60).toString().padStart(2, '0');
    const seconds = (totalSeconds % 60).toString().padStart(2, '0');
    return `${minutes}:${seconds}`;
}

// Función para actualizar el contenido del cuadro
function updateStep() {
    const currentStep = steps[currentStepIndex];
    stepDescription.textContent = currentStep.description;

    // Limpiar imágenes
    const imagesContainer = document.querySelector(".images-container");
    imagesContainer.innerHTML = "";
    currentStep.images.forEach((imageSrc, index) => {
        const img = document.createElement("img");
        img.src = imageSrc;
        img.alt = `Imatge del pas ${currentStep.id} - ${index + 1}`;
        img.classList.add("step-image");
        imagesContainer.appendChild(img);
    });

    // Mostrar u ocultar el temporizador y botón según paso y estado
    if (currentStepIndex >= 5 && currentStepIndex <= 13) {
        // Mostrar contenedor
        timerContainer.style.display = "flex";

        if (currentStepIndex === 5) {
            // Paso 6: Reinicio con 15:00
            resetTimer(15 * 60, true);
            timerManuallyFinalized = false;
        } else if (currentStepIndex >= 6 && currentStepIndex <= 10) {
            if (!timerManuallyFinalized) {
                timerBtn.style.display = "inline-block"; // Siempre visible si no ha sido finalizado
            } else {
                timerContainer.style.display = "none"; // Ocultar si fue finalizado
            }
        } else if (currentStepIndex === 11) {
            // Paso 12: forzar 00:00 y ocultar botón
            stopTimer(0, false);
        } else if (currentStepIndex === 12) {
            // Paso 13: reiniciar con 30:00
            resetTimer(30 * 60, true);
            timerManuallyFinalized = false;
        } else if (currentStepIndex === 13) {
            // Paso 14: forzar 00:00
            stopTimer(0, false);
        }
    } else {
        timerContainer.style.display = "none";
    }


    // Botones de navegación
    prevButton.disabled = currentStepIndex === 0;
    nextButton.textContent = currentStepIndex === steps.length - 1 ? "Finalitzar" : "Següent →";
}

timerBtn.addEventListener("click", () => {
    if (!timerStarted) {
        timerStarted = true;
        timerBtn.textContent = "Finalitzar";

        fakeInterval = setInterval(() => {
            fakeTimeLeft--;
            timerDisplay.textContent = formatTime(fakeTimeLeft);

            if (fakeTimeLeft <= 0) {
                clearInterval(fakeInterval);
                timerDisplay.textContent = "00:00";
                timerBtn.style.display = "none";
                timerStarted = false;
                timerManuallyFinalized = true;
            }
        }, 1000);
    } else {
        // Se pulsa "Finalitzar"
        clearInterval(fakeInterval);
        fakeTimeLeft = 0;
        timerDisplay.textContent = "00:00";
        timerBtn.style.display = "none";
        timerStarted = false;
        timerManuallyFinalized = true;
    }
});

// Event listeners para los botones
prevButton.addEventListener("click", () => {
    if (currentStepIndex > 0) {
        currentStepIndex--;
        updateStep();
    }
});

nextButton.addEventListener("click", () => {
    if (currentStepIndex === steps.length - 1) {
        // Redirigir a inici.html si es el último paso
        window.location.href = "../inici.html";
    } else {
        currentStepIndex++;
        updateStep();
    }
});

// Inicializar el contenido del cuadro
updateStep();