from pydantic import BaseModel, Field
from typing import List, Optional, Dict, Any
from datetime import datetime
from uuid import UUID

class MissionEvent(BaseModel):
    event_type: str
    timestamp: datetime = Field(default_factory=datetime.utcnow)
    details: Dict[str, Any] = Field(default_factory=dict)

class Mission(BaseModel):
    id: Optional[str] = Field(None, alias="_id")
    dron_id: UUID
    start_time: datetime
    end_time: Optional[datetime] = None
    status: str # "scheduled", "ongoing", "completed", "aborted"
    payload_type: str # "package", "sensor", "camera"
    mission_objective: str
    logs: List[MissionEvent] = []
    
    class Config:
        allow_population_by_field_name = True
        schema_extra = {
            "example": {
                "dron_id": "550e8400-e29b-41d4-a716-446655440000",
                "start_time": "2023-01-01T10:00:00Z",
                "status": "ongoing",
                "payload_type": "package",
                "mission_objective": "Deliver medical supplies to Sector 7",
                "logs": [
                    {
                        "event_type": "takeoff",
                        "timestamp": "2023-01-01T10:05:00Z",
                        "details": {"altitude": 50, "battery": 98.5}
                    }
                ]
            }
        }

class MissionCreate(BaseModel):
    dron_id: UUID
    payload_type: str
    mission_objective: str
    start_time: datetime = Field(default_factory=datetime.utcnow)

class MissionUpdate(BaseModel):
    end_time: Optional[datetime] = None
    status: Optional[str] = None
    
class MissionEventCreate(BaseModel):
    event_type: str
    details: Dict[str, Any] = Field(default_factory=dict)
