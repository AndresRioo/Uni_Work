from rest_framework import serializers
from .models import Player, Game, Board, Vessel, BoardVessel, Shot
from django.contrib.auth.models import User

class UserSerializer(serializers.ModelSerializer):
    password = serializers.CharField(write_only=True)
    partidas_jugadas = serializers.SerializerMethodField()
    partidas_ganadas = serializers.SerializerMethodField()

    class Meta:
        model = User
        fields = ['username', 'email', 'password', 'partidas_jugadas', 'partidas_ganadas']
        extra_kwargs = {'password': {'write_only': True}}

    def create(self, validated_data): 
        # sobreescribir metodo create para que no use el metodo create, sino create_user para que este hasheada y django la acepte
        return User.objects.create_user(
            username=validated_data['username'],
            email=validated_data['email'],
            password=validated_data['password']
        )
    
    def get_player_id(self, obj):
        return obj.player.id if hasattr(obj, 'player') else None
    
    def get_partidas_jugadas(self, obj):
        return obj.player.partidas_jugadas if hasattr(obj, 'player') else 0

    def get_partidas_ganadas(self, obj):
        return obj.player.partidas_ganadas if hasattr(obj, 'player') else 0

class PlayerSerializer(serializers.ModelSerializer):
    class Meta:
        model = Player
        fields = '__all__'

class GameSerializer(serializers.ModelSerializer):
    game_state_response = serializers.SerializerMethodField()

    class Meta:
        model = Game
        fields = ['id', 'phase', 'turn', 'winner', 'game_state_response']

    def get_game_state_response(self, obj):
        return GameStateResponseSerializer(obj, context=self.context).data
    
class GameStateResponseSerializer(serializers.Serializer):
    status = serializers.IntegerField(default=200)
    message = serializers.CharField(default="OK")
    data = serializers.SerializerMethodField()

    def get_data(self, obj):
        return {
            'gameState': GameStateSerializer(obj, context=self.context).data
        }

    

class PlayerStateSerializer(serializers.Serializer):
    id = serializers.CharField(source='owner.id')
    username = serializers.CharField(source='owner.nickname')
    placedShips = serializers.SerializerMethodField()
    # availableShips = serializers.SerializerMethodField()
    board = serializers.SerializerMethodField()

    def get_placedShips(self, board):
        ships = []
        for vessel in BoardVessel.objects.filter(board=board):
            ships.append({
                'type': vessel.vessel.id,
                'position': {
                    'row': vessel.ri,
                    'col': vessel.ci
                },
                'isVertical': vessel.rf != vessel.ri,
                'size': vessel.vessel.size
            })
        return ships

    def get_availableShips(self, board):
        all_vessels = Vessel.objects.all()
        placed_vessels = BoardVessel.objects.filter(board=board)
        ships = []

        for vessel in all_vessels:
            if not placed_vessels.filter(vessel=vessel).exists():
                ships.append({
                    'type': vessel.id,
                    'isVertical': True,
                    'size': vessel.size
                })
        return ships

    def get_board(self, board):
        width = board.game.width
        height = board.game.height
        grid = [[0 for _ in range(width)] for _ in range(height)]

        # Afegeix els vaixells
        for vessel in BoardVessel.objects.filter(board=board):
            value = vessel.vessel.id if vessel.alive else vessel.vessel.id
            for i in range(min(vessel.ri, vessel.rf), max(vessel.ri, vessel.rf) + 1):
                for j in range(min(vessel.ci, vessel.cf), max(vessel.ci, vessel.cf) + 1):
                    grid[i][j] = value

        # Afegeix els trets
        for shot in Shot.objects.filter(board=board):
            if grid[shot.row][shot.col] == 0:
                grid[shot.row][shot.col] = 11  # Fallado
            else:
                grid[shot.row][shot.col] = -grid[shot.row][shot.col]  # Tocado, se pone en negativo


        return grid


class GameStateSerializer(serializers.Serializer):
    gameId = serializers.CharField(source='id')
    phase = serializers.CharField()
    turn = serializers.CharField(source='turn.nickname', allow_null=True)
    winner = serializers.CharField(source='winner.nickname', allow_null=True)
    player1 = serializers.SerializerMethodField()
    player2 = serializers.SerializerMethodField()

    def get_player1(self, obj):
        players = obj.players.order_by('id')
        if players.count() < 1:
            return None
        board = obj.boards.filter(owner=players[0]).first()
        if not board:
            return None
        return PlayerStateSerializer(board).data

    def get_player2(self, obj):
        players = obj.players.order_by('id')
        if players.count() < 2:
            return None
        board = obj.boards.filter(owner=players[1]).first()
        if not board:
            return None
        return PlayerStateSerializer(board).data



    


class BoardSerializer(serializers.ModelSerializer):
    owner_nickname = serializers.ReadOnlyField(source='owner.nickname')

    class Meta:
        model = Board
        fields = '__all__'

class VesselSerializer(serializers.ModelSerializer):
    type = serializers.IntegerField(source='board_value')
    class Meta:
        model = Vessel
        fields = ['type', 'size', 'name']


class BoardVesselSerializer(serializers.ModelSerializer):
    vessel_name = serializers.ReadOnlyField(source='vessel.name')

    class Meta:
        model = BoardVessel
        fields = '__all__'

class ShotSerializer(serializers.ModelSerializer):
    player_nickname = serializers.ReadOnlyField(source='player.nickname')

    class Meta:
        model = Shot
        fields = '__all__'






class PlacedShipSerializer(serializers.Serializer):
    type = serializers.IntegerField()
    position = serializers.DictField(child=serializers.IntegerField())
    isVertical = serializers.BooleanField()
    size = serializers.IntegerField(read_only=True)



    def validate(self, data):
        
        pos = data['position']
        if 'row' not in pos or 'col' not in pos:
            raise serializers.ValidationError("position must have row and col")
        return data

    # ESTO SOLO TRANSFORMA DE COORDENADAS
    def create(self, validated_data):
        board = self.context.get('board')
        if not board:
            raise serializers.ValidationError("board context is required")

        try:
            vessel_obj = Vessel.objects.get(board_value=validated_data['type'])


        except Vessel.DoesNotExist:
            raise serializers.ValidationError("Vessel type does not exist (CREATE DE PLACEDSHIPSERIALIZER)")

        size = vessel_obj.size
        ri = validated_data['position']['row']
        ci = validated_data['position']['col']
        is_vertical = validated_data['isVertical']

        rf = 0
        cf = 0

        if is_vertical:
            rf = ri + size - 1
            cf = ci
        else:
            ci = ci - (size - 1)  # Ajustamos hacia la izquierda
            rf = ri
            cf = ci + size - 1


        # Validación de límites del tablero
        game = board.game
        if not (0 <= ri <= rf < game.height) or not (0 <= ci <= cf < game.width):

            print(f"DEBUG: ri={ri}, ci={ci}, rf={rf}, cf={cf}, size={size}, is_vertical={is_vertical}")
            print(f"DEBUG: game.height={game.height}, game.width={game.width}")

            raise serializers.ValidationError("El barco no cabe en el tablero")

        # Validación de solape
        for existing in BoardVessel.objects.filter(board=board):
            print(f"DEBUG EXISTING: ri={existing.ri}, rf={existing.rf}, ci={existing.ci}, cf={existing.cf}")
            print(f"DEBUG NEW: ri={ri}, rf={rf}, ci={ci}, cf={cf}")

            if not (cf < existing.ci or ci > existing.cf or rf < existing.ri or ri > existing.rf):
                print("DEBUG: Se detecta solape con barco existente")
                raise serializers.ValidationError("El barco se solapa con otro ya colocado")

        return BoardVessel.objects.create(
            vessel=vessel_obj,
            board=board,
            ri=ri,
            ci=ci,
            rf=rf,
            cf=cf,
            alive=True
        )







