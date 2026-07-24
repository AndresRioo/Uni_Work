from fastapi import FastAPI
from app.drons.controller import router as drons_router
from app.telemetria.router import router as telemetria_router

from app.rabbitmq_client import rabbitmq_client
from app.cassandra_client import cassandra_client

app = FastAPI(title="Dron Logistics API - Messaging Edition", version="0.5.0")
    

app.include_router(drons_router)
app.include_router(telemetria_router)

@app.get("/")
def index():
    return {"name": app.title, "version": app.version}
