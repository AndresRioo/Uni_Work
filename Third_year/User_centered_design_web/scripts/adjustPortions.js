function initializePortionAdjuster({
    portionCountElementId,
    decreaseButtonId,
    increaseButtonId,
    ingredientSelector,
    originalQuantities,
    initialPortions
}) {
    const portionCountElement = document.getElementById(portionCountElementId);
    const decreaseButton = document.getElementById(decreaseButtonId);
    const increaseButton = document.getElementById(increaseButtonId);
    const ingredientElements = document.querySelectorAll(ingredientSelector);

    let currentPortions = initialPortions;

    // Actualiza las cantidades de los ingredientes
    function updateIngredients() {
        ingredientElements.forEach((element, index) => {
            const original = originalQuantities[index];
            const newValue = (original.value / initialPortions) * currentPortions;
            const textWithoutNumber = original.text.replace(/^\d+(\.\d+)?\s*/, '');
            const displayValue = Number.isInteger(newValue) ? newValue : newValue.toFixed(1);
            element.textContent = `${displayValue} ${textWithoutNumber}`;
        });
    }

    // Maneja el incremento de porciones
    increaseButton.addEventListener('click', () => {
        currentPortions++;
        portionCountElement.textContent = currentPortions;
        updateIngredients();
    });

    // Maneja el decremento de porciones
    decreaseButton.addEventListener('click', () => {
        if (currentPortions > 1) {
            currentPortions--;
            portionCountElement.textContent = currentPortions;
            updateIngredients();
        }
    });

    // Inicializa las cantidades de los ingredientes
    updateIngredients();
}