// Variables para el modal
const modal = document.getElementById('modal');
const yesButton = document.getElementById('yes-button');
const noButton = document.getElementById('no-button');
let recipeToDelete = null;

// Función para mostrar el modal
function showModal(recipeId) {
    recipeToDelete = document.getElementById(recipeId); // Guarda la receta que se quiere eliminar
    modal.style.display = 'block'; // Muestra el modal
}

// Manejar el clic en "Sí"
yesButton.addEventListener('click', () => {
    if (recipeToDelete) {
        const recipeName = recipeToDelete.querySelector('h3').textContent; // Obtiene el nombre de la receta
        recipeToDelete.parentNode.removeChild(recipeToDelete); // Elimina la receta del DOM

        // Mostrar un mensaje temporal con el nombre de la receta
        const toastMessage = document.getElementById('toast-message');
        toastMessage.textContent = `Recepta "${recipeName}" eliminada de favorits.`;
        toastMessage.style.display = 'block';

        // Ocultar el mensaje después de 3 segundos
        setTimeout(() => {
            toastMessage.style.display = 'none';
        }, 3000);
    }
    modal.style.display = 'none'; // Oculta el modal
});

// Manejar el clic en "No"
noButton.addEventListener('click', () => {
    modal.style.display = 'none'; // Oculta el modal
});