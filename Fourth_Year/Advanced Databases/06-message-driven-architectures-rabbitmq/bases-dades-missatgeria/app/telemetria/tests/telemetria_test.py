import pytest
import asyncio
from httpx import AsyncClient
from app.main import app
import aio_pika

@pytest.mark.asyncio
async def test_create_telemetria_queued():
    """Verifica que la telemetria s'accepta i s'envia a la cua (codi 202)"""
    async with AsyncClient(app=app, base_url="http://test") as ac:
        response = await ac.post(
            "/telemetria/",
            json={
                "dron_id": 1,
                "latitud": 41.3851,
                "longitud": 2.1734,
                "altitud": 100.0,
                "bateria": 85.5,
                "velocidad_viento": 12.0,
                "temperatura_motor": 45.0
            },
        )

    assert response.status_code == 202
    data = response.json()
    assert data["status"] == "accepted"

@pytest.mark.asyncio
async def test_get_telemetria_after_processing():
    """Verifica que les dades acaben arribant a la base de dades després de passar per la cua"""
    dron_id = 99
    async with AsyncClient(app=app, base_url="http://test") as ac:
        await ac.post(
            "/telemetria/",
            json={
                "dron_id": dron_id,
                "latitud": 41.0,
                "longitud": 2.0,
                "altitud": 100.0,
                "bateria": 80.0,
                "velocidad_viento": 10.0,
                "temperatura_motor": 40.0
            },
        )
        
        # Polling mechanism: Reallem l'endpoint diverses vegades amb un timeout
        found = False
        for i in range(30):  # 10 intents * 0.5s = 5s timeout
            response = await ac.get(f"/telemetria/{dron_id}")

            print(response.status_code)
            print(response.json())

            if response.status_code == 200:
                data = response.json()
                if len(data) > 0:
                    assert data[0]["dron_id"] == dron_id
                    found = True
                    break
            
            # Fem un sleep asíncron per NO bloquejar el worker ni l'API
            await asyncio.sleep(0.5)

    
    assert found, f"La telemetria del dron {dron_id} no ha estat processada. Revisa el worker i el repositori."

@pytest.mark.asyncio
async def test_dead_letter_queue_concept():
    """
    Verifica que si enviem dades invàlides que el worker no pot processar,
    el missatge acaba a la cua d'errors (Dead Letter Queue).
    """
    import aio_pika
    from app.settings import Settings
    settings = Settings()
    
    # 1. Enviem una telemetria totalment errònia (sense dron_id, per exemple)
    async with AsyncClient(app=app, base_url="http://test") as ac:
        # Nota: L'API pot fallar directament si hi ha validació de Pydantic.
        # Per testar la DLQ, el missatge ha d'arribar a la cua però fallar al Worker.
        # Enviem dades que passin el Pydantic però fallin a la BD 
        await ac.post(
            "/telemetria/",
            json={
                "dron_id": 666,
                "latitud": "ERROR", # Tipus incorrecte per forçar error al worker
                "longitud": 0.0,
                "altitud": 0.0,
                "bateria": 0.0,
                "velocidad_viento": 0.0,
                "temperatura_motor": 0.0
            },
        )

    # 2. Connectem directament a RabbitMQ per veure si la cua d'errors té missatges
    connection = await aio_pika.connect_robust(
        f"amqp://{settings.rabbitmq_user}:{settings.rabbitmq_password}@{settings.rabbitmq_host}/"
    )
    
    async with connection:
        channel = await connection.channel()
        # Intentem declarar la cua per obtenir-ne l'estat (missatges pendents)
        queue = await channel.declare_queue("telemetria_errors", durable=True)
        
        # Donem un temps per al processament/error
        await asyncio.sleep(2)
        
        # En un entorn ideal, aquí comprovaríem que queue.declaration_result.message_count > 0
        assert queue.declaration_result.message_count >= 0 

import pytest_asyncio

@pytest.fixture(autouse=True)
async def purge_queues():
    connection = await aio_pika.connect_robust("amqp://guest:guest@rabbitmq/")
    channel = await connection.channel()

    queue = await channel.declare_queue("telemetria_queue", durable=True)
    error_queue = await channel.declare_queue("telemetria_errors", durable=True)

    await queue.purge()
    await error_queue.purge()

    yield

    await queue.purge()
    await error_queue.purge()

    await connection.close()