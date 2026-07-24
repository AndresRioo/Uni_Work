from fastapi import APIRouter, HTTPException, Query, Depends
from typing import List, Optional
from datetime import datetime
from uuid import UUID
from cassandra import ConsistencyLevel
from app.telemetria.models import Telemetria, TelemetriaCreate
from app.telemetria.repository import TelemetriaRepository
from typing import Union

router = APIRouter(prefix="/telemetria", tags=["telemetria"])

def get_repo():
    return TelemetriaRepository()

@router.post("/", response_model=Telemetria)
async def create_telemetria(
    telemetria: TelemetriaCreate, 
    consistency: Optional[str] = Query("ONE"),
    repo: TelemetriaRepository = Depends(get_repo)
):
    try:
        # Mapegem el string a l'enum de Cassandra
        c_level = getattr(ConsistencyLevel, consistency.upper(), ConsistencyLevel.ONE)
        return repo.save(telemetria, consistency_level=c_level)
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@router.get("/{dron_id}", response_model=List[Telemetria])
async def get_telemetria(
    dron_id: Union[UUID, int],
    start: Optional[datetime] = Query(None),
    end: Optional[datetime] = Query(None),
    repo: TelemetriaRepository = Depends(get_repo)
):
    try:
        return repo.get_by_dron(dron_id, start, end)
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@router.get("/stats/{dron_id}")
async def get_stats(dron_id: Union[UUID, int], repo: TelemetriaRepository = Depends(get_repo)):
    try:
        stats = repo.get_stats(dron_id)
        if not stats:
            raise HTTPException(status_code=404, detail="No stats found for this drone")
        return stats
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
