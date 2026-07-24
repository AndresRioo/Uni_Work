from fastapi import HTTPException
from sqlalchemy.orm import Session
from typing import List, Optional
from uuid import UUID

from . import models, schemas

def get_dron(db: Session, dron_id: UUID) -> Optional[models.Dron]:
    return db.query(models.Dron).filter(models.Dron.id == dron_id).first()

def get_dron_by_name(db: Session, name: str) -> Optional[models.Dron]:
    return db.query(models.Dron).filter(models.Dron.name == name).first()

"""
Original - Punt 2: Crear una ruta GET per a obtenir tots els drons
#TODO: Implement this method to return all drons from the database
def get_drons(db: Session, skip: int = 0, limit: int = 100) -> List[models.Dron]:
    return []
"""


def get_drons(db: Session, skip: int = 0, limit: int = 100) -> List[models.Dron]:
    return db.query(models.Dron).offset(skip).limit(limit).all()



def create_dron(db: Session, dron: schemas.DronCreate) -> models.Dron:
    db_dron = models.Dron(name=dron.name, latitude=dron.latitude, longitude=dron.longitude)
    db.add(db_dron)
    db.commit()
    db.refresh(db_dron)
    return db_dron

def record_data(db: Session, dron_id: UUID, data: schemas.DronData) -> models.Dron:
    db_dron = db.query(models.Dron).filter(models.Dron.id == dron_id).first()
    if not db_dron:
        raise HTTPException(status_code=404, detail="Dron not found")
        
    # Business Logic: Check battery if trying to fly
    if data.status == "en_vol" and data.battery_level < 20:
        raise HTTPException(status_code=400, detail="Cannot fly with battery below 20%")
        
    db_dron.battery_level = data.battery_level
    db_dron.last_seen = data.last_seen
    db_dron.status = data.status
    
    db.commit()
    db.refresh(db_dron)
    return db_dron

def delete_dron(db: Session, dron_id: UUID):
    db_dron = db.query(models.Dron).filter(models.Dron.id == dron_id).first()
    if db_dron is None:
        raise HTTPException(status_code=404, detail="Dron not found")
    db.delete(db_dron)
    db.commit()
    return db_dron