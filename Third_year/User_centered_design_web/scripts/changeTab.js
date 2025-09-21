const recipesTab = document.getElementById('recipes-tab');
const commentsTab = document.getElementById('comments-tab');
const recipesContent = document.getElementById('recipes-content');
const commentsContent = document.getElementById('comments-content');

recipesTab.addEventListener('click', () => {
    recipesTab.classList.add('active');
    commentsTab.classList.remove('active');
    recipesContent.style.display = 'grid';
    commentsContent.style.display = 'none';
});

commentsTab.addEventListener('click', () => {
    commentsTab.classList.add('active');
    recipesTab.classList.remove('active');
    recipesContent.style.display = 'none';
    commentsContent.style.display = 'grid';
});