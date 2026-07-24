from datetime import datetime
from pydantic import BaseModel
from uuid import UUID

class TelemetriaBase(BaseModel):
    dron_id: int
    latitud: float
    longitud: float
    altitud: float
    bateria: float
    velocidad_viento: float
    temperatura_motor: float

class TelemetriaCreate(TelemetriaBase):
    pass

class Telemetria(TelemetriaBase):
    timestamp: datetime

    class Config:
        orm_mode = True
