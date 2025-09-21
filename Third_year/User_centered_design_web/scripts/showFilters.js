const filterButton = document.getElementById('filter-button');
const filterPanel = document.getElementById('filter-panel');
const closeFilters = document.getElementById('close-filters');
const acceptFilters = document.getElementById('accept-filters');

// Mostrar/ocultar la pestaña de filtros al pulsar el botón "Filtres"
filterButton.addEventListener('click', () => {
    if (filterPanel.style.display === 'block') {
        filterPanel.style.display = 'none';
    } else {
        filterPanel.style.display = 'block';
    }
});

// Cerrar la pestaña de filtros al pulsar la "X"
closeFilters.addEventListener('click', () => {
    filterPanel.style.display = 'none';
});

// Redirigir a la URL especificada en data-destination al pulsar "Aceptar"
acceptFilters.addEventListener('click', () => {
    const destination = acceptFilters.getAttribute('data-destination');
    if (destination) {
        window.location.href = destination;
    } else {
        console.error('No se especificó un destino en el atributo data-destination.');
    }
});