

# ESTRUCTURA DE REDIS


1. Estat Operacional (Hashes): Gestiona la telemetria (bateria, estat) en memòria per a respostes de baixa latència.

- Los ids a guardar : dron:{UUID}
- 

2. Rànquings en Temps Real (Sorted Sets): Implementa la lògica de rànquings d'eficiència utilitzant estructures de dades natives de Redis.



3. Indexació Espacial de Proximitat (GEO): Resol consultes de discovery geogràfic sense penalitzar la base de dades relacional.


# Ejecutar tests

Solo de `controller_test.py`

docker exec bdda_api sh -c "cd /app && pytest -x"


# IMPORTANTE

redis_client = redis.Redis(
    host=settings.redis_host,
    port=settings.redis_port,
    db=settings.redis_db,
    decode_responses=True
)

ya decodifica solo, no poner decode

# BRUNO

http://localhost:8000/drons

{
  "name": "Dron BCN",
  "latitude": 41.38,
  "longitude": 2.17
}

RESPUESTA ESPERADA

{
  "id": "a3f0c3b4-3f12-4b67-9c9e-41df0b19b8aa",
  "name": "Dron BCN",
  "latitude": 41.38,
  "longitude": 2.17,
  "status": "disponible",
  "joined_at": "2026-02-20T19:12:01.123Z",
  "last_seen": "2026-02-20T19:12:01.123Z",
  "battery_level": 100.0,
  "last_telemetry": null,
  "technical_info": null
}


http://localhost:8000/drons/{UUID}/data

POST
http://localhost:8000/drons/a3f0c3b4-3f12-4b67-9c9e-41df0b19b8aa/data

{
  "battery_level": 85.0,
  "last_seen": "2023-01-01T12:00:00.000Z",
  "status": "en_vol"
}

En redis

HGETALL dron:a3f0c3b4-3f12-4b67-9c9e-41df0b19b8aa
- status = en_vol
- battery_level = 85.0

ZRANGE drons_battery_leaderboard 0 -1 WITHSCORES
- debe incluir el UUID con score 85.0

GEOPOS drons_geo a3f0c3b4-3f12-4b67-9c9e-41df0b19b8aa


http://localhost:8000/drons/{UUID}

GET
http://localhost:8000/drons/a3f0c3b4-3f12-4b67-9c9e-41df0b19b8aa

Respuesta esperada

{
  "id": "a3f0c3b4-3f12-4b67-9c9e-41df0b19b8aa",
  "name": "Dron BCN",
  "latitude": 41.38,
  "longitude": 2.17,
  "status": "en_vol",
  "joined_at": "2026-02-20T19:12:01.123Z",
  "last_seen": "2023-01-01T12:00:00.000Z",
  "battery_level": 85.0,
  "last_telemetry": "...",
  "technical_info": null
}


# ENTRAR A LA TERMINAL DE REDIS

docker exec -it bdda_redis redis-cli

## Comandos a ejecutar

KEYS * 