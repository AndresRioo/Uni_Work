const modalOverlay = document.getElementById('modal-overlay');
const modal = document.getElementById('success-modal');
const closeModalButton = document.getElementById('close-modal');
const form = document.querySelector('.form-container');

// Interceptar el envío del formulario
form.addEventListener('submit', (event) => {
    event.preventDefault(); // Evita el envío real del formulario
    modalOverlay.style.display = 'block'; // Muestra el fondo semitransparente
    modal.style.display = 'block'; // Muestra el modal
});

// Cerrar el modal al hacer clic en el botón "Tancar"
closeModalButton.addEventListener('click', () => {
    modalOverlay.style.display = 'none'; // Oculta el fondo semitransparente
    modal.style.display = 'none'; // Oculta el modal
});