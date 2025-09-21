from django.contrib.auth.models import User
from . import models
from rest_framework import viewsets, filters, permissions
from django.shortcuts import get_object_or_404

from .models import Player, Game, Board, Vessel, BoardVessel, Shot
from .serializers import PlayerSerializer, GameSerializer, UserSerializer, BoardSerializer, VesselSerializer, BoardVesselSerializer, ShotSerializer, PlacedShipSerializer

from rest_framework.response import Response

from rest_framework import status
from rest_framework.response import Response
from django.shortcuts import get_object_or_404
import traceback
from django.db import connection
from rest_framework.decorators import action



class UserViewSet(viewsets.ModelViewSet):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    filter_backends = [filters.SearchFilter]
    search_fields = ['username', 'email']
    permission_classes = [permissions.AllowAny] #para el registro

    def partial_update(self, request, *args, **kwargs):
        user = request.user
        player = getattr(user, 'player', None)
        if not player:
            return Response({'error': 'Player no encontrado para este usuario'}, status=404)
        
        bot_player = Player.objects.get(nickname='bot')

        if not bot_player:
            print("error con el bot, no se inicializa bien")


        action = request.data.get('action')
        if action == 'partida': # gana el bot
            player.refresh_from_db()  # recarga del estado de la DB
            player.partidas_jugadas += 1

            bot_player.partidas_jugadas += 1
            bot_player.partidas_ganadas += 1

            player.save()
            bot_player.save()

            print(f"No hubo victoria, action -> {action}")
        elif action == 'victoria': # gana el user
            player.refresh_from_db()  # recarga del estado de la DB
            player.partidas_jugadas += 1
            player.partidas_ganadas += 1

            bot_player.partidas_jugadas += 1

            player.save()
            bot_player.save()

            print(f"Sí hubo victoria, action -> {action}")
        else:
            return Response({'error': 'Acción no reconocida'}, status=400)

        return Response({
            'partidas_jugadas': player.partidas_jugadas,
            'partidas_ganadas': player.partidas_ganadas
        }, status=200)
    
    # conseguir user id
    @action(detail=False, methods=["get"], permission_classes=[permissions.IsAuthenticated])
    def me(self, request):
        serializer = self.get_serializer(request.user)
        return Response(serializer.data)


class PlayerViewSet(viewsets.ModelViewSet):
    queryset = Player.objects.all()
    serializer_class = PlayerSerializer
    filter_backends = [filters.SearchFilter]
    search_fields = ['nickname']

class GameViewSet(viewsets.ModelViewSet):
    queryset = Game.objects.all()
    serializer_class = GameSerializer

    def get_serializer_context(self):
            context = super().get_serializer_context()
            context['request'] = self.request
            return context
    
    def create(self, request, *args, **kwargs):
        try:
            serializer = self.get_serializer(data=request.data)
            serializer.is_valid(raise_exception=True)
            game = serializer.save()
            player = request.user.player
            game.players.add(player)
            Board.objects.create(game=game, owner=player)

            bot_player = Player.objects.get(nickname='bot')
            game.players.add(bot_player)
            Board.objects.create(game=game, owner=bot_player)

            return Response(serializer.data, status=status.HTTP_201_CREATED)
        
        except Exception as e:
            traceback.print_exc()
            return Response({"error": str(e)}, status=509)

    # patch para cambiar de estado
    def partial_update(self, request, *args, **kwargs):
        try:
            instance = self.get_object()
            data = request.data.copy()

            # Detectar si están usando "player1" o "player2" como alias
            alias_fields = ["turn", "winner"]
            for field in alias_fields:
                value = data.get(field)
                if value == "player1":
                    # Usamos el primer jugador asociado
                    player = instance.players.all().order_by("id").first()
                    if player:
                        data[field] = player.id
                elif value == "player2":
                    # Segundo jugador asociado
                    players = instance.players.all().order_by("id")
                    if players.count() >= 2:
                        data[field] = players[1].id

            serializer = self.get_serializer(instance, data=data, partial=True)
            serializer.is_valid(raise_exception=True)
            self.perform_update(serializer)
            return Response(serializer.data, status=status.HTTP_200_OK)

        except Exception as e:
            import traceback
            traceback.print_exc()
            return Response({"error": str(e)}, status=509)



    def retrieve(self, request, *args, **kwargs):
        instance = self.get_object()
        serializer = self.get_serializer(instance, context={'request': request})
        return Response(serializer.data)




class BoardViewSet(viewsets.ModelViewSet):
    queryset = Board.objects.all()
    serializer_class = BoardSerializer
    filter_backends = [filters.SearchFilter, filters.OrderingFilter]
    search_fields = ['prepared']
    ordering_fields = ['board_id']

class VesselViewSet(viewsets.ModelViewSet):
    queryset = Vessel.objects.all()
    serializer_class = VesselSerializer
    filter_backends = [filters.SearchFilter, filters.OrderingFilter]
    search_fields = ['name']
    ordering_fields = ['size']


"""
class ShotViewSet(viewsets.ModelViewSet):
    queryset = Shot.objects.all()
    serializer_class = ShotSerializer
    filter_backends = [filters.SearchFilter, filters.OrderingFilter]
    search_fields = ['result']
    ordering_fields = ['row', 'col']

    def create(self, request, game_pk=None, player_pk=None):
        print("Entrando en create() de ShotViewSet")

        try:
            player = get_object_or_404(Player, pk=player_pk)
            game = get_object_or_404(Game, pk=game_pk)
            print(f"Recibido game: {game}, player: {player}")

            row = request.data.get('row')
            col = request.data.get('col')
            print(f"Disparo en fila {row}, columna {col}")

            if game.turn != player:
                print("No es el turno de este jugador")
                return Response({'error': 'No es tu turno'}, status=400)

            opponent = game.players.exclude(id=player.id).first()
            print(f"Oponente: {opponent}")

            if not opponent:
                print("No hay oponente")
                return Response({'error': 'No hay oponente'}, status=400)

            opponent_board = Board.objects.get(game=game, owner=opponent)
            print(f"Board del oponente: {opponent_board}")

            # if Shot.objects.filter(board=opponent_board, row=row, col=col).exists():
            #     print("Disparo repetido")
            #     return Response({'error': 'Ya has disparado a esa casilla'}, status=400)

            # tampoco se deberia de disparar al mismo sitio pero el frontend no guarda na

            vessel_hit = BoardVessel.objects.filter(
                board=opponent_board,
                ri__lte=row, rf__gte=row,
                ci__lte=col, cf__gte=col
            ).first()
            result = 'hit' if vessel_hit else 'miss'
            print(f"Resultado del disparo: {result}")

            if result == 'miss':
                result_value = 0
            
            if result == 'hit':
                result_value = 1

            if result == 'sunk':
                result_value = 2

            shot = Shot.objects.create(
                player=player,
                board=opponent_board,
                game=game,
                row=row,
                col=col,
                result=result_value,
            )
            print(f"Shot creado: {shot}")

            #if result == 'miss': 
            # EN TEORIA SE CAMBIA DE JUGADOR PERO MANTENEMOS LOGICA ORIGINAL (PREGUNTAR EN CLASE)
            
            game.turn = opponent
            game.save()
            print(f"Turno cambiado a: {opponent}")

            serializer = self.get_serializer(shot)
            print(f"Serialización OK")
            return Response(serializer.data, status=201)

        except Exception as e:
            import traceback
            print("Error en create()")
            traceback.print_exc()
            return Response({'error': str(e)}, status=500)
"""


class ShotViewSet(viewsets.ModelViewSet):
    queryset = Shot.objects.all()
    serializer_class = ShotSerializer
    filter_backends = [filters.SearchFilter, filters.OrderingFilter]
    search_fields = ['result']
    ordering_fields = ['row', 'col']

    def create(self, request, game_pk=None, player_pk=None):
        print("Entrando en create() de ShotViewSet")

        try:
            player = get_object_or_404(Player, pk=player_pk)
            game = get_object_or_404(Game, pk=game_pk)
            print(f"Recibido game: {game}, player: {player}")

            row = int(request.data.get('row'))
            col = int(request.data.get('col'))
            print(f"Disparo en fila {row}, columna {col}")

            if game.turn != player:
                return Response({'error': 'No es tu turno'}, status=400)

            opponent = game.players.exclude(id=player.id).first()
            if not opponent:
                return Response({'error': 'No hay oponente'}, status=400)

            opponent_board = Board.objects.get(game=game, owner=opponent)

            vessel_hit = BoardVessel.objects.filter(
                board=opponent_board,
                ri__lte=row, rf__gte=row,
                ci__lte=col, cf__gte=col
            ).first()

            # crear primer el shot per verificar si es sunk o game over
            shot = Shot.objects.create(
                player=player,
                board=opponent_board,
                game=game,
                row=row,
                col=col,
                result=0, #valor temporal
            )


            if vessel_hit:
                is_sunk = self.check_if_sunk(vessel_hit, opponent_board, row, col)
                if is_sunk:
                    result = 'sunk'
                    result_value = 2
                else:
                    result = 'hit'
                    result_value = 1
            else:
                result = 'miss'
                result_value = 0
                # game.turn = opponent # cambiar de turno solo si se hace miss

            print(f"Resultado del disparo: {result}")

            # Comprobamos si ha ganado
            game_over = self.check_if_game_over(opponent_board)
            if game_over:
                game.phase = "GameOver"
                game.winner = player
                result_value = 3 # codigo para indicar fin de partida
                game.save()
                print(f"Partida finalizada. Ganador: {player}")
                
            else:
                game.turn = opponent
                game.save()

            # actualitzar el result_value del shot
            shot.result = result_value
            shot.save()

            
            serializer = self.get_serializer(shot)
            data = serializer.data

            return Response(data, status=201)

        except Exception as e:
            import traceback
            traceback.print_exc()
            return Response({'error': str(e)}, status=500)

    def check_if_sunk(self, vessel, board, row, col):
        """
        Comprueba si el barco `vessel` ha sido hundido tras el disparo en (row, col).
        """
        vessel_cells = [
            (r, c)
            for r in range(vessel.ri, vessel.rf + 1)
            for c in range(vessel.ci, vessel.cf + 1)
        ]

        shots = Shot.objects.filter(
            board=board,
            row__in=[r for r, _ in vessel_cells],
            col__in=[c for _, c in vessel_cells]
        )

        shot_cells = set((shot.row, shot.col) for shot in shots)
        shot_cells.add((row, col))  # añadir disparo actual

        return all(cell in shot_cells for cell in vessel_cells)

    def check_if_game_over(self, board):
        """
        Comprueba si todos los barcos han sido hundidos.
        Si alguna parte de un barco no ha sido alcanzada por un disparo, el juego continúa.
        """
        vessels = BoardVessel.objects.filter(board=board)

        for vessel in vessels:
            for row in range(vessel.ri, vessel.rf + 1):
                for col in range(vessel.ci, vessel.cf + 1):
                    if not Shot.objects.filter(board=board, row=row, col=col).exists():
                        print(f"Juego NO terminado: casilla sin disparar en fila {row}, columna {col}")
                        return False  # Parte del barco sigue viva

        print("Todos los barcos han sido hundidos. Juego terminado.")
        return True



# /games/{game_pk}/players/
class GamePlayerViewSet(viewsets.ModelViewSet):
    serializer_class = PlayerSerializer

    def get_queryset(self):
        game_id = self.kwargs['game_pk']
        game = get_object_or_404(Game, pk=game_id)
        return game.players.all()

    def create(self, request, *args, **kwargs):
        game = get_object_or_404(Game, pk=self.kwargs['game_pk'])
        player = request.user.player  
        game.players.add(player)
        Board.objects.get_or_create(game=game, owner=player)

        game.save()

        # Aquí serializas la partida completa
        from .serializers import GameSerializer
        serializer = GameSerializer(game, context={'request': request})

        return Response(serializer.data, status=status.HTTP_201_CREATED)
    

class BoardVesselViewSet(viewsets.ViewSet):

    def create(self, request, game_pk=None, player_pk=None):

        player = get_object_or_404(Player, pk=player_pk)
        game = get_object_or_404(Game, pk=game_pk)
        board = Board.objects.filter(game=game, owner=player).first()

        if not player:
            return Response({"detail": "Player no encontrado"}, status=status.HTTP_404_NOT_FOUND)
        
        if not game:
            return Response({"detail": "Game no encontrado"}, status=status.HTTP_404_NOT_FOUND)

        if not board:
            return Response({"detail": "Board no encontrado"}, status=status.HTTP_404_NOT_FOUND)
        

        ships_data = request.data  

        serializer = PlacedShipSerializer(data=ships_data, context={'board': board})
        serializer.is_valid(raise_exception=True)
        serializer.save()

        from .serializers import GameSerializer
        game_serializer = GameSerializer(game, context={'request': request})
        return Response(game_serializer.data, status=status.HTTP_200_OK)




