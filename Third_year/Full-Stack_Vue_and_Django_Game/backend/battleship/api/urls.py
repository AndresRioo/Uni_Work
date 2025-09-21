from django.urls import path, include
from rest_framework.routers import DefaultRouter
from .views import PlayerViewSet, GameViewSet, UserViewSet, BoardViewSet, VesselViewSet, BoardVesselViewSet, ShotViewSet, GamePlayerViewSet
from rest_framework_nested.routers import NestedSimpleRouter

router = DefaultRouter()
router.register(r'players', PlayerViewSet)
router.register(r'games', GameViewSet)
router.register(r'user', UserViewSet)

urlpatterns = [
    path('', include(router.urls)),
]

# /games/{game_pk}/players/
games_players_router = NestedSimpleRouter(router, r'games', lookup='game')
games_players_router.register(r'players', GamePlayerViewSet, basename='game-players')


# /games/{game_pk}/players/{player_pk}/(vessels|shots|boards)/
nested_player_router = NestedSimpleRouter(games_players_router, r'players', lookup='player')
nested_player_router.register(r'vessels', BoardVesselViewSet, basename='game-player-vessels')
nested_player_router.register(r'shots', ShotViewSet, basename='game-player-shots')
nested_player_router.register(r'boards', BoardViewSet, basename='game-player-boards')

urlpatterns = [
    path('', include(router.urls)),
    path('', include(games_players_router.urls)),
    path('', include(nested_player_router.urls)),
]