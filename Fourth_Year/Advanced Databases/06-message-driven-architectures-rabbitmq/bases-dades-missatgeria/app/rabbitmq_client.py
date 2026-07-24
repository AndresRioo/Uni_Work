import aio_pika
import json
from app.settings import Settings

settings = Settings()

class RabbitMQClient:
    def __init__(self):
        self.connection = None
        self.channel = None

    async def connect(self):
        self.connection = await aio_pika.connect_robust(
            f"amqp://{settings.rabbitmq_user}:{settings.rabbitmq_password}@{settings.rabbitmq_host}/"
        )
        self.channel = await self.connection.channel()

        # TODO: Declarar l'exchange principal de tipus TOPIC
        self.exchange = await self.channel.declare_exchange(
            "telemetria_exchange",
            aio_pika.ExchangeType.TOPIC,
            durable=True
        )

        # TODO: Declarar la Dead Letter Exchange (DLX) de tipus DIRECT
        self.dlx = await self.channel.declare_exchange(
            "telemetria_dlx",
            aio_pika.ExchangeType.DIRECT,
            durable=True
        )

        # TODO: Declarar la cua 'telemetria_queue' i vincular-la a la DLX
        self.queue = await self.channel.declare_queue(
            "telemetria_queue",
            durable=True,
            arguments={
                "x-dead-letter-exchange": "telemetria_dlx",
                "x-dead-letter-routing-key": "telemetria.error"
            }
        )

        # TODO: Fer el bind de la cua a l'exchange principal amb la routing_key adequada
        await self.queue.bind(
            self.exchange,
            routing_key="drone.*.telemetria"
        )

        # TODO: Declarar la cua d'errors 'telemetria_errors' i vincular-la a la DLX
        self.error_queue = await self.channel.declare_queue(
            "telemetria_errors",
            durable=True
        )

        await self.error_queue.bind(
            self.dlx,
            routing_key="telemetria.error"
        )

    async def publish_telemetria(self, dron_id: int, data: dict):

        connection = await aio_pika.connect_robust(
            f"amqp://{settings.rabbitmq_user}:{settings.rabbitmq_password}@{settings.rabbitmq_host}/"
        )

        async with connection:

            channel = await connection.channel()

            exchange = await channel.declare_exchange(
                "telemetria_exchange",
                aio_pika.ExchangeType.TOPIC,
                durable=True
            )

            message = aio_pika.Message(
                body=json.dumps(data).encode(),
                delivery_mode=aio_pika.DeliveryMode.PERSISTENT,
            )

            await exchange.publish(
                message,
                routing_key=f"drone.{dron_id}.telemetria"
            )

            print("PUBLICADO OK:", dron_id)

    async def close(self):
        if self.connection:
            await self.connection.close()

rabbitmq_client = RabbitMQClient()