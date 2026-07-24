from fastapi import APIRouter, Depends, HTTPException
from typing import List
from .models import Mission, MissionCreate, MissionUpdate, MissionEventCreate, MissionEvent
from .repository import MissionRepository
from ..mongodb_client import get_mongodb
from uuid import UUID

router = APIRouter(
    prefix="/missions",
    tags=["missions"],
)

async def get_repository():
    db = get_mongodb()
    return MissionRepository(db)

@router.post("", response_model=str)
async def create_mission(mission: MissionCreate, repo: MissionRepository = Depends(get_repository)):
    return await repo.create_mission(mission)

@router.get("/dron/{dron_id}", response_model=List[dict])
async def get_dron_missions(dron_id: UUID, repo: MissionRepository = Depends(get_repository)):
    return await repo.get_missions_by_dron(dron_id)

@router.post("/{mission_id}/events")
async def add_mission_event(mission_id: str, event: MissionEventCreate, repo: MissionRepository = Depends(get_repository)):
    success = await repo.add_event_to_mission(mission_id, MissionEvent(**event.dict()))
    if not success:
        raise HTTPException(status_code=404, detail="Mission not found")
    return {"status": "event added"}

@router.patch("/{mission_id}")
async def update_mission(mission_id: str, update: MissionUpdate, repo: MissionRepository = Depends(get_repository)):
    success = await repo.update_mission_status(mission_id, update)
    if not success:
        raise HTTPException(status_code=404, detail="Mission not found")
    return {"status": "updated"}

@router.get("/search")
async def search_missions(q: str, repo: MissionRepository = Depends(get_repository)):
    return await repo.search_missions_by_objective(q)
