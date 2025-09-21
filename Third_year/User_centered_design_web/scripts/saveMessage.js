const saveIcon = document.querySelector('.save-icon'); // Selecciona el botón "Guardar"
const toastMessage = document.getElementById('toast-message'); // Selecciona el contenedor del mensaje temporal

saveIcon.addEventListener('click', () => {
    // Mostrar el mensaje temporal
    toastMessage.textContent = 'Recepta guardada als teus favorits.';
    toastMessage.style.display = 'block';

    // Ocultar el mensaje después de 3 segundos
    setTimeout(() => {
        toastMessage.style.display = 'none';
    }, 3000);
});