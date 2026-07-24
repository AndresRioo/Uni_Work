from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from uuid import UUID
from typing import List

from . import models, schemas, repository
from app.database import SessionLocal, engine

# Dependency
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

router = APIRouter(
    prefix="/drons",
    responses={404: {"description": "Not found"}},
    tags=["drons"],
)

@router.get("", response_model=List[schemas.Dron])
def get_drons(skip: int = 0, limit: int = 100, db: Session = Depends(get_db)):
    #return [100]
    return repository.get_drons(db, skip, limit)

@router.get("/{dron_id}", response_model=schemas.Dron)
def get_dron(dron_id: UUID, db: Session = Depends(get_db)):
    db_dron = repository.get_dron(db, dron_id)
    if db_dron is None:
        raise HTTPException(status_code=404, detail="Dron not found")
    return db_dron

@router.post("", response_model=schemas.Dron)
def create_dron(dron: schemas.DronCreate, db: Session = Depends(get_db)):
    db_dron = repository.get_dron_by_name(db, dron.name)
    if db_dron:
        raise HTTPException(status_code=400, detail="Dron with same name already registered")
    return repository.create_dron(db=db, dron=dron)

@router.post("/{dron_id}/data", response_model=schemas.Dron)
def record_data(dron_id: UUID, data: schemas.DronData, db: Session = Depends(get_db)):
    return repository.record_data(db=db, dron_id=dron_id, data=data)

@router.delete("/{dron_id}")
def delete_dron(dron_id: UUID, db: Session = Depends(get_db)):
    return repository.delete_dron(db=db, dron_id=dron_id)
    
