function assignLibretaListeners() {
    document.getElementById('libreta-1').addEventListener('click', () => {
        const recipesContent = document.getElementById('recipes-content');
        const backButton = document.getElementById('back-button');

        // Reemplazar el contenido con un nuevo grid de recetas
        recipesContent.innerHTML = `
            <div class="grid-item">
                <a href="../receptes/receptaBrownies.html" style="text-decoration: none; color: inherit;">
                    <img src="../imatges/brownies_chocolate.jpg" alt="Brownies de xocolata">
                    <h3>Brownies de xocolata</h3>
                    <div class="tags">
                        <span class="tag">Sense gluten</span>
                        <span class="tag">Sense sucre</span>
                        <span class="tag">Sense lactis</span>
                    </div>
                    <div class="stars">★★★★★</div>
                </a>
            </div>
            <div class="grid-item">
                <img src="imatges/croissants_fresa.jpg" alt="Croissants amb maduixes">
                <h3>Croissants amb maduixes</h3>
                <div class="tags">
                    <span class="tag">Vegà</span>
                </div>
                <div class="stars">★★★☆☆</div>
            </div>
            <div class="grid-item">
                <a href="../receptes/receptaMagdalenes.html" style="text-decoration: none; color: inherit;">
                    <img src="../imatges/cupcakes_basic.jpg" alt="Magdalenes">
                    <h3>Magdalenes</h3>
                    <div class="stars">★★★★☆</div>
                </a>
            </div>
        `;

        // Mostrar el botón "Volver"
        backButton.style.display = 'inline-block';

        // Reasignar los eventos de arrastrar y soltar
        assignDragAndDropListeners();
    });
}

document.addEventListener('DOMContentLoaded', () => {
    const recipesContent = document.getElementById('recipes-content');
    const backButton = document.getElementById('back-button');
    const originalContent = recipesContent.innerHTML; // Guarda el contenido original del grid

    // Asignar los event listeners iniciales
    assignLibretaListeners();
    assignDragAndDropListeners();

    // Listener para el botón "Volver"
    backButton.addEventListener('click', () => {
        // Restaurar el contenido original del grid
        recipesContent.innerHTML = originalContent;

        // Ocultar el botón "Volver"
        backButton.style.display = 'none';

        // Reasignar los event listeners a las libretas y drag & drop
        assignLibretaListeners();
        assignDragAndDropListeners();
    });
});

function assignDragAndDropListeners() {
    document.querySelectorAll('.grid-item.libreta').forEach(libreta => {
        libreta.ondragover = (event) => event.preventDefault(); // Permitir el arrastre
        libreta.ondrop = handleDrop; // Asignar la función de manejo de la caída
    });

    document.querySelectorAll('.grid-item[draggable="true"]').forEach(item => {
        item.addEventListener('dragstart', (event) => {
            event.dataTransfer.setData('text/plain', event.target.id); // Guarda el ID de la receta
        });
    });
}