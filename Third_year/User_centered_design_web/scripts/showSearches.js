const searchInput = document.getElementById('search-input');
const recentList = document.getElementById('recent-list');

// Mostrar la lista al hacer clic en el input
searchInput.addEventListener('focus', () => {
    recentList.style.display = 'block';
});

// Ocultar la lista al hacer clic fuera del input
searchInput.addEventListener('blur', () => {
    setTimeout(() => {
        recentList.style.display = 'none';
    }, 200); // Retraso para permitir clics en los elementos
});

function navigateTo(item) {
    alert(`Navegando a: ${item}`);
    // Aquí puedes redirigir a otra página o mostrar contenido relacionado
}