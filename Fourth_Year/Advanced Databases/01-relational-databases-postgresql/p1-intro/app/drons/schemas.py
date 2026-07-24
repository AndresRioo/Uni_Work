from pydantic import BaseModel, validator
from typing import Optional
from uuid import UUID
from datetime import datetime

class Dron(BaseModel):
    id: UUID
    name: str
    latitude: float
    longitude: float
    status: str
    joined_at: datetime
    last_seen: datetime
    battery_level: float
    last_telemetry: Optional[datetime] = None
    technical_info: Optional[str] = None
    
    class Config:
        orm_mode = True
        
class DronCreate(BaseModel):
    name: str
    longitude: float
    latitude: float
    
    @validator('name')
    def name_not_empty(cls, v):
        if not v or not v.strip():
            raise ValueError('Name cannot be empty')
        return v

class DronData(BaseModel):
    battery_level: float
    last_seen: datetime
    status: str
    
    @validator('battery_level')
    def battery_level_valid(cls, v):
        if not (0 <= v <= 100):
            raise ValueError('Battery level must be between 0 and 100')
        return v