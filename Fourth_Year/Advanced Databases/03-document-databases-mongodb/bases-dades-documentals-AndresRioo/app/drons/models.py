import datetime
from sqlalchemy import Column, DateTime, Float, String
from sqlalchemy.dialects.postgresql import UUID
import uuid
from ..database import Base

class Dron(Base):
    __tablename__ = "drons"
    id = Column(UUID(as_uuid=True), primary_key=True, index=True, default=uuid.uuid4)
    name = Column(String, unique=True, index=True)
    latitude = Column(Float)
    longitude = Column(Float)
    status = Column(String, default="disponible") # disponible, en_vol, retornant, bateria_baixa, manteniment
    joined_at = Column(DateTime, default=datetime.datetime.utcnow)
    last_seen = Column(DateTime, default=datetime.datetime.utcnow)
    battery_level = Column(Float, default=100.0)
    last_telemetry = Column(DateTime, nullable=True)
    technical_info = Column(String, nullable=True)
    