import asyncio
import json
import logging
import aio_pika
from app.settings import Settings
from app.telemetria.repository import TelemetriaRepository
from app.telemetria.models import TelemetriaCreate
from uuid import UUID

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

settings = Settings()


from app.redis_client import get_redis
from datetime import datetime

redis = get_redis()


async def process_message(message: aio_pika.IncomingMessage):
    """
    TODO: Implementar el processament del missatge.
    1. Extraure les dades del body (JSON).
    2. Guardar la telemetria a Cassandra.
    3. Actualitzar l'estat del dron a Redis.
    """
    
    async with message.process():
        try:

            data = json.loads(message.body.decode())

            # Convertir a model
            telemetria = TelemetriaCreate(**data)

            logger.info(f"Mensaje recibido: {data}")

            # Crear repositorio para cada mensaje
            repo = TelemetriaRepository()

            # Guardar en Cassandra
            repo.save(telemetria)

            logger.info("Guardado en Cassandra")


            # Actualitzar estat a Redis
            # Convertir dron_id a UUID (igual que Cassandra)
            dron_uuid = telemetria.dron_id
            if isinstance(dron_uuid, int):
                dron_uuid = UUID(int=dron_uuid)

            key = f"dron:{dron_uuid}"

            # Actualizar estado en Redis (hash)
            redis.hset(key, mapping={
                "last_seen": datetime.utcnow().isoformat(),
                "battery_level": telemetria.bateria,
                "last_telemetry": datetime.utcnow().isoformat()
            })

            # Leaderboard
            redis.zadd("drons_battery_leaderboard", {
                str(dron_uuid): telemetria.bateria
            })

        except Exception as e:
            logger.error(f"Error procesando mensaje: {e}")
            raise


async def main():
    connection = await aio_pika.connect_robust(
        f"amqp://{settings.rabbitmq_user}:{settings.rabbitmq_password}@{settings.rabbitmq_host}/"
    )

    channel = await connection.channel()


    # Declarar l'exchange principal de tipus TOPIC
    # S'utilitza per enrutar la telemetria segons la routing key
    exchange = await channel.declare_exchange(
        "telemetria_exchange",
        aio_pika.ExchangeType.TOPIC,
        durable=True
    )


    # Declarar la Dead Letter Exchange (DLX)
    # Rebrà els missatges que fallin durant el processament
    dlx = await channel.declare_exchange(
        "telemetria_dlx",
        aio_pika.ExchangeType.DIRECT,
        durable=True
    )


    # Declarar la cua principal de telemetria
    # Configurada amb DLX per enviar-hi els missatges erronis
    queue = await channel.declare_queue(
        "telemetria_queue",
        durable=True,
        arguments={
            "x-dead-letter-exchange": "telemetria_dlx",
            "x-dead-letter-routing-key": "telemetria.error"
        }
    )

    # Vincular la cua principal a l'exchange TOPIC
    # Rebrà qualsevol routing key del tipus:
    # drone.<id>.telemetria
    await queue.bind(
        exchange,
        routing_key="drone.*.telemetria"
    )

    # Declarar la cua d'errors (Dead Letter Queue)
    error_queue = await channel.declare_queue(
        "telemetria_errors",
        durable=True
    )

    # Vincular la DLQ a la Dead Letter Exchange
    await error_queue.bind(
        dlx,
        routing_key="telemetria.error"
    )


    # Començar a consumir missatges de la cua principal
    # Cada missatge serà processat per la funció process_message
    await queue.consume(process_message)

    logger.info("Worker escoltant missatges...")

    await asyncio.Future()

if __name__ == "__main__":
    asyncio.run(main())
