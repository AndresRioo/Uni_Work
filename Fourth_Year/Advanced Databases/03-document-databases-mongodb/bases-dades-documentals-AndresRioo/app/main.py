from fastapi import FastAPI
from app.drons.controller import router as drons_router
# TODO: Import the missions router
from app.missions.router import router as missions_router

from app.mongodb_client import MongoDBClient

app = FastAPI(title="Dron Logistics API - Polyglot Edition", version="0.3.0")

@app.on_event("shutdown")
async def shutdown():
    MongoDBClient.close_connection()

app.include_router(drons_router)
# TODO: Include the missions router
app.include_router(missions_router)

@app.get("/")
def index():
    return {"name": app.title, "version": app.version}
