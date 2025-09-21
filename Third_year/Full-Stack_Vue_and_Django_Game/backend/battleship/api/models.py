from django.db import models
from django.contrib.auth.models import User
from django.core.validators import MinValueValidator, MaxValueValidator

class Player(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    nickname = models.CharField(max_length=50)
    partidas_jugadas = models.PositiveIntegerField(default=0)
    partidas_ganadas = models.PositiveIntegerField(default=0)


class Game(models.Model):
    PHASE_WAITING = "waiting"
    PHASE_PLACEMENT = "placement"
    PHASE_PLAYING = "playing"
    PHASE_GAMEOVER = "gameOver"
    PHASE_CHOICES = {
        PHASE_WAITING: "Waiting",
        PHASE_PLACEMENT: "Placement",
        PHASE_PLAYING: "Playing",
        PHASE_GAMEOVER: "Game Over",
    }

    players = models.ManyToManyField(Player, related_name="games")
    width = models.IntegerField(validators=[MinValueValidator(5), MaxValueValidator(200)], default=10)
    height = models.IntegerField(validators=[MinValueValidator(5), MaxValueValidator(200)], default=10)
    multiplayer = models.BooleanField(default=False)
    turn = models.ForeignKey(Player, related_name="turn", on_delete=models.SET_NULL, blank=True, null=True)
    phase = models.CharField(max_length=15, choices=PHASE_CHOICES.items(), default=PHASE_WAITING)
    winner = models.ForeignKey(Player, related_name="winner", on_delete=models.SET_NULL, blank=True, null=True)
    owner = models.ForeignKey(Player, related_name="owner", on_delete=models.SET_NULL, null=True)

class Board(models.Model):
    board_id = models.AutoField(primary_key=True)
    prepared = models.BooleanField(default=False)
    owner = models.ForeignKey(Player, related_name="boards", on_delete=models.CASCADE)
    game = models.ForeignKey(Game, related_name="boards", on_delete=models.CASCADE)

class Vessel(models.Model):
    size = models.IntegerField(validators=[MinValueValidator(1)])
    name = models.CharField(max_length=50)
    image = models.URLField(blank=True, null=True)
    board_value = models.IntegerField(default=0)
    
class BoardVessel(models.Model):
    vessel = models.ForeignKey(Vessel, related_name="board_vessels", on_delete=models.CASCADE)
    board = models.ForeignKey(Board, related_name="board_vessels", on_delete=models.CASCADE)
    ri = models.IntegerField()
    ci = models.IntegerField()
    rf = models.IntegerField()
    cf = models.IntegerField()
    alive = models.BooleanField(default=True)

class Shot(models.Model):
    RESULT_MISS = 0
    RESULT_HIT = 1
    RESULT_SUNK = 2

    RESULT_CHOICES = [
        (RESULT_MISS, "miss"),
        (RESULT_HIT, "hit"),
        (RESULT_SUNK, "sunk"),
    ]
    row = models.IntegerField()
    col = models.IntegerField()
    result = models.IntegerField(choices = RESULT_CHOICES)  # 0: miss, 1: hit, 2: sunk
    player = models.ForeignKey(Player, related_name="shots", on_delete=models.CASCADE)
    game = models.ForeignKey(Game, related_name="shots", on_delete=models.CASCADE)
    board = models.ForeignKey(Board, related_name="shots", on_delete=models.CASCADE)
    boardVessel = models.ForeignKey(BoardVessel, related_name="shots", on_delete=models.SET_NULL, blank=True, null=True)

