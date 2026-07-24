from fastapi import FastAPI
from app.drons.controller import router as drons_router

app = FastAPI(title="Dron Logistics API", version="0.1.0")

app.include_router(drons_router)

@app.get("/")
def index():
    return {"name": app.title, "version": app.version}
