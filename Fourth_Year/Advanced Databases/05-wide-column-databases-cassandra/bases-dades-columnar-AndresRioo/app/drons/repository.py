from fastapi import HTTPException
from sqlalchemy.orm import Session
from typing import List, Optional
from uuid import UUID

from . import models, schemas

from ..redis_client import get_redis
import json

from datetime import datetime


redis = get_redis()


def get_dron(db: Session, dron_id: UUID) -> Optional[models.Dron]:
    """
    TODO: Implement logic to retrieve a drone. 
    Requirement: You MUST merge the static data from PostgreSQL with the dynamic data from Redis.
    In case of conflict or if Redis data is missing, handle fallbacks gracefully.
    """

    # Fuente de verdad: Postgres
    dron = db.query(models.Dron).filter(models.Dron.id == dron_id).first()
    if dron is None:
        return None

    # Leer Redis (hash del dron)
    key = f"dron:{dron_id}"
    redis_data = redis.hgetall(key)

    # Función para unir postgre y redis
    _merge_redis_state(dron, redis_data)

    return dron


def get_dron_by_name(db: Session, name: str) -> Optional[models.Dron]:
    return db.query(models.Dron).filter(models.Dron.name == name).first()

def get_drons(db: Session, skip: int = 0, limit: int = 100) -> List[models.Dron]:
    """
    TODO: Implement this method to return all drons from the database.
    Note: For a high-performance system, consider how to minimize calls to Redis when listing multiple entities.
    """

    drons = db.query(models.Dron).offset(skip).limit(limit).all()
    if not drons:
        return drons

    # Construir las keys de Redis
    keys = [f"dron:{d.id}" for d in drons]

    # HACER SOLO 1 PASADA POR REDIS
    pipe = redis.pipeline()
    for key in keys:
        pipe.hgetall(key)
    redis_results = pipe.execute()

    # Merge por posición
    for dron, redis_data in zip(drons, redis_results):
        _merge_redis_state(dron, redis_data)

    return drons


def _merge_redis_state(dron, redis_data):
    """
    Función auxiliar para get_dron() y get_drons() para evitar codigo duplicado al unir la info de postgre y redis
    """
    if not redis_data:
        return dron  # no hay datos dinámicos , devolver lo que hy

    # Merge por campo del contenido de redis
    # Si falla, saltar campo
    if "battery_level" in redis_data:
        try:
            dron.battery_level = float(redis_data["battery_level"])
        except Exception:
            pass

    if "status" in redis_data:
        try:
            dron.status = redis_data["status"]
        except Exception:
            pass

    if "last_seen" in redis_data:
        try:
            dron.last_seen = datetime.fromisoformat(redis_data["last_seen"])
        except Exception:
            pass

    if "last_telemetry" in redis_data:
        try:
            dron.last_telemetry = datetime.fromisoformat(redis_data["last_telemetry"])
        except Exception:
            pass
    return dron

def create_dron(db: Session, dron: schemas.DronCreate) -> models.Dron:
    """
    TODO: Initialize the drone in BOTH databases.
    Requirement: 
    - Save initial metadata to PostgreSQL.
    - Initialize the operational state (status, battery) in a Redis Hash.
    - Ensure the drone is searchable by location using Redis Geospatial indexes.
    """
    # PostgreSQL
    db_dron = models.Dron(name=dron.name, latitude=dron.latitude, longitude=dron.longitude)
    db.add(db_dron)
    db.commit()
    db.refresh(db_dron)

    # Redis Hash DATOS POR DEFAULT, AÑADIR INFO ACTUALIZADA EN RECORD DATA
    key = f"dron:{db_dron.id}"
    redis.hset(key, mapping={
        "status": db_dron.status,                 # "disponible"
        "battery_level": db_dron.battery_level,   # 100.0
        "last_seen": db_dron.last_seen.isoformat()
    })

    # Redis GEO index (lon, lat, member)
    redis.geoadd("drons_geo", (db_dron.longitude, db_dron.latitude, str(db_dron.id)))

    return db_dron



def record_data(db: Session, dron_id: UUID, data: schemas.DronData) -> models.Dron:
    """
    TODO: The core of the real-time system.
    Requirement:
    - Validate business logic (battery vs status).
    - Persist permanent metadata (like 'last_seen') to PostgreSQL.
    - Update the 'hot' state in Redis Hashes.
    - Update the centralized 'Electric Efficiency' leaderboard (Sorted Set).
    - Ensure the updated object returned to the user contains the latest data from BOTH sources.
    """
    db_dron = db.query(models.Dron).filter(models.Dron.id == dron_id).first()
    if not db_dron:
        raise HTTPException(status_code=404, detail="Dron not found")
        
    # Business Logic: No puede volar con bateria baja
    if data.status == "en_vol" and data.battery_level < 20:
        raise HTTPException(status_code=400, detail="Cannot fly with battery below 20%")
    
    # Actualizar 'hot-state' en los hashes de redis
    key = f"dron:{dron_id}"
    redis.hset(key, mapping={
        "status": data.status,                
        "battery_level": data.battery_level, 
        "last_seen": data.last_seen.isoformat()
    })

    # Update a la bateria (Rànquings en Temps Real (Sorted Sets))
    redis.zadd("drons_battery_leaderboard", mapping={
        str(dron_id): data.battery_level
    } )

    # Persist permanent metadata (like 'last_seen') to PostgreSQL.
    # Principio de “persist only what is permanent”
    
    # db_dron.status = data.status
    # db_dron.battery_level = data.battery_level
    db_dron.last_seen = data.last_seen

    db.add(db_dron)
    db.commit()

    return get_dron(db, dron_id)

def delete_dron(db: Session, dron_id: UUID):
    """
    TODO: Perform a clean deletion.
    Requirement: 
    - Remove the entity from PostgreSQL.
    - Purge ALL associated data from Redis (Hash, Sorted Set, and GEO Index).
    Failing to clean Redis will lead to 'phantom drones' in your system!
    """
    db_dron = db.query(models.Dron).filter(models.Dron.id == dron_id).first()
    if db_dron is None:
        raise HTTPException(status_code=404, detail="Dron not found")
    db.delete(db_dron)
    db.commit()

    # Borrar en Redis con pipeline para ser más eficiente y evitar llamadas extra 
    key = f"dron:{dron_id}"
    pipe = redis.pipeline()
    pipe.delete(key)
    pipe.zrem("drons_battery_leaderboard", str(dron_id))
    pipe.zrem("drons_geo", str(dron_id))
    pipe.execute()

    return db_dron

def get_leaderboard(db: Session, limit: int = 10):
    """
    TODO: Retrieve the top N drones by battery level.
    Requirement: This MUST be serviced entirely by Redis Sorted Sets to guarantee sub-millisecond latency.
    The response should include basic drone info (id, name, battery_level).
    """
    
    # Top N desde Redis (descendente por score)
    top = redis.zrevrange("drons_battery_leaderboard", 0, limit - 1, withscores=True)

    if not top:
        return []

    # Hacer solo 1 query a postgre para no tener N+1 Query Problem
    # Extraer IDs 
    ids = [UUID(dron_id) for dron_id, _ in top]

    # Una sola query a postgre
    drons = db.query(models.Dron).filter(models.Dron.id.in_(ids)).all()

    # Crear mapa id → dron
    dron_map = {d.id: d for d in drons}

    results = []

    # Devolver el ranking con los datos
    for dron_id_str, battery in top:
        dron_id = UUID(dron_id_str)
        dron = dron_map.get(dron_id)

        if not dron:
            redis.zrem("drons_battery_leaderboard", str(dron_id)) # eliminar dato fantasma para mantener coherencia
            continue

        results.append({
            "id": str(dron.id),
            "name": dron.name,
            "battery_level": float(battery)
        })

    return results

def search_by_location(db: Session, longitude: float, latitude: float, radius_km: float):
    """
    TODO: Implement proximity-based drone discovery.
    Requirement: Use Redis Geospatial features. Returning drones that no longer exist in Postgres 
    is a failure condition; ensure cross-database consistency.
    """

    ids = redis.geosearch(
        "drons_geo",
        longitude=longitude,
        latitude=latitude,
        radius=radius_km,
        unit="km"
    )

    if not ids:
        return []

    # Hacer solo 1 query a postgre para no tener N+1 Query Problem
    uuid_ids = [UUID(dron_id) for dron_id in ids]
    drons = db.query(models.Dron).filter(models.Dron.id.in_(uuid_ids)).all()
    dron_map = {d.id: d for d in drons}

    results = []

    # devolver datos por orden de localizacion
    for dron_id_str in ids:
        dron_id = UUID(dron_id_str)
        dron = dron_map.get(dron_id)

        if not dron:
            redis.zrem("drons_geo", str(dron_id)) # eliminar dato fantasma para mantener coherencia
            continue

        results.append(dron)

    return results