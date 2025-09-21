// Manejar el inicio del arrastre
document.querySelectorAll('.grid-item[draggable="true"]').forEach(item => {
    item.addEventListener('dragstart', (event) => {
        event.dataTransfer.setData('text/plain', event.target.id); // Guarda el ID de la receta
    });
});

// Manejar la caída en una libreta
function handleDrop(event) {
    event.preventDefault();
    const recipeId = event.dataTransfer.getData('text/plain'); // Obtiene el ID de la receta
    const recipeElement = document.getElementById(recipeId); // Encuentra el elemento arrastrado

    if (recipeElement) {
        // Elimina la receta del grid
        recipeElement.parentNode.removeChild(recipeElement);

        // Obtener el nombre de la libreta
        const libretaElement = event.target.closest('.grid-item.libreta'); // Encuentra la libreta donde se soltó
        const libretaName = libretaElement.querySelector('.libreta-title').textContent; // Obtiene el título de la libreta

        // Mostrar el mensaje temporal
        const toastMessage = document.getElementById('toast-message');
        toastMessage.textContent = `Afegida la recepta "${recipeElement.querySelector('h3').textContent}" a la llibreta "${libretaName}"`;
        toastMessage.style.display = 'block';

        // Ocultar el mensaje después de 3 segundos
        setTimeout(() => {
            toastMessage.style.display = 'none';
        }, 3000);
    }
}