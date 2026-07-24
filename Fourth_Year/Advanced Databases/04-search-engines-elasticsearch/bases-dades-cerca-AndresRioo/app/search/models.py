from pydantic import BaseModel, Field
from typing import List, Optional, Dict, Any
from datetime import datetime
from uuid import UUID

class IncidentReport(BaseModel):
    id: Optional[str] = None
    dron_id: UUID
    timestamp: datetime = Field(default_factory=datetime.utcnow)
    severity: str # "low", "medium", "high", "critical"
    component: str # "motor", "battery", "gps", "camera", "software"
    description: str # Full text incident details
    environment_conditions: Dict[str, Any] = {} # e.g. {"wind_speed": 20, "temp": 35}
    pilot_notes: Optional[str] = None

class SearchQuery(BaseModel):
    text: str
    severity: Optional[str] = None
    component: Optional[str] = None
    fuzzy: bool = False
