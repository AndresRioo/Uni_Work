from fastapi import FastAPI
from app.drons.controller import router as drons_router
from app.telemetria.router import router as telemetria_router

from app.cassandra_client import cassandra_client

app = FastAPI(title="Dron Logistics API - Cassandra Edition", version="0.4.0")

@app.on_event("startup")
async def startup():
    await cassandra_client.connect()

@app.on_event("shutdown")
async def shutdown():
    cassandra_client.close()

app.include_router(drons_router)
app.include_router(telemetria_router)

@app.get("/")
def index():
    return {"name": app.title, "version": app.version}
