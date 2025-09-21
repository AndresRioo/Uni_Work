const interactiveCookingButton = document.getElementById('interactive-cooking-button');
const modal = document.getElementById('modal');
const yesButton = document.getElementById('yes-button');
const noButton = document.getElementById('no-button');
const closeModalButton = document.querySelector('.close-modal');

// Mostrar el modal al pulsar el botón
interactiveCookingButton.addEventListener('click', (event) => {
    event.preventDefault(); // Evita la redirección predeterminada
    modal.style.display = 'block';
});

// Redirigir al inicio si elige "Sí"
yesButton.addEventListener('click', () => {
    window.location.href = '../inici.html';
});

// Redirigir a cuinaInteractiva.html si elige "No"
noButton.addEventListener('click', () => {
    window.location.href = 'cuinaInteractiva.html';
});

// Cerrar el modal al hacer clic en la "X"
closeModalButton.addEventListener('click', () => {
    modal.style.display = 'none';
});