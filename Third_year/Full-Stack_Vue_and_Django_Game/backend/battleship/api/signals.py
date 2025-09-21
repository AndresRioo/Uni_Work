from django.db.models.signals import post_save
from django.contrib.auth.models import User
from django.dispatch import receiver
from .models import Player
from django.db.models.signals import post_save, post_migrate
from django.contrib.auth import get_user_model
from .models import Vessel

@receiver(post_save, sender=User)
def create_player(sender, instance, created, **kwargs):
    if created:
        Player.objects.create(user=instance, nickname=instance.username)

# Crea los barcos si no existen (solo una vez, tras migraciones)
@receiver(post_migrate)
def create_initial_vessels(sender, **kwargs):
    initial_data = [
        {"size": 1, "name": "Vessel tipo 1", "board_value": 1},
        {"size": 2, "name": "Vessel tipo 2", "board_value": 2},
        {"size": 3, "name": "Vessel tipo 3", "board_value": 3},
        {"size": 4, "name": "Vessel tipo 4", "board_value": 4},
        {"size": 5, "name": "Vessel tipo 5", "board_value": 5},
    ]

    for vessel_data in initial_data:
        Vessel.objects.get_or_create(
            board_value=vessel_data["board_value"],
            defaults={
                "size": vessel_data["size"],
                "name": vessel_data["name"]
            }
        )

    print("Instancias de barco creadas ")

@receiver(post_migrate)
def create_default_superuser(sender, **kwargs):
    User = get_user_model()
    if not User.objects.filter(is_superuser=True).exists():
        User.objects.create_superuser(
            username='bot',
            email='bot@example.com',
            password='NoEntresConElBotQueNoTeDejaJugaraA#@mifakPPOKFE@###NoEntresConElBotQueNoTeDejaJugar@1RFDVSVaisocnaov'
        )
        print("Superuser 'bot' creado ")
