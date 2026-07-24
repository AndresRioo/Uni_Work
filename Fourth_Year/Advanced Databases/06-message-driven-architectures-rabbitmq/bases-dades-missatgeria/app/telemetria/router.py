from fastapi import APIRouter, HTTPException, Query, Depends
from typing import List, Optional
from datetime import datetime
from cassandra import ConsistencyLevel
from app.telemetria.models import Telemetria, TelemetriaCreate
from app.telemetria.repository import TelemetriaRepository
from uuid import UUID

from app.rabbitmq_client import rabbitmq_client

router = APIRouter(prefix="/telemetria", tags=["telemetria"])

def get_repo():
    return TelemetriaRepository()

@router.post("/", status_code=202)
async def create_telemetria(
    telemetria: TelemetriaCreate
):
    """
    Publica la telemetria a RabbitMQ per al seu processament asíncron.
    Retorna 202 (Accepted) per indicar que la petició s'ha rebut i s'està processant.
    """
    try:
        await rabbitmq_client.publish_telemetria(telemetria.dron_id, telemetria.dict())
        return {"status": "accepted"}
        # return {"status": "accepted", "message": "Telemetria enviada a la cua de processament"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@router.get("/{dron_id}", response_model=List[Telemetria])
async def get_telemetria(
    dron_id: int,
    start: Optional[datetime] = Query(None),
    end: Optional[datetime] = Query(None),
    repo: TelemetriaRepository = Depends(get_repo)
):
    try:
        return repo.get_by_dron(dron_id, start, end)
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@router.get("/stats/{dron_id}")
async def get_stats(dron_id: int, repo: TelemetriaRepository = Depends(get_repo)):
    try:
        stats = repo.get_stats(dron_id)
        if not stats:
            raise HTTPException(status_code=404, detail="No stats found for this drone")
        return stats
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
