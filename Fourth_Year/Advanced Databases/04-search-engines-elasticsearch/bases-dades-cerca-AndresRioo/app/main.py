from fastapi import FastAPI
from app.drons.controller import router as drons_router
from app.missions.router import router as missions_router
from app.search.router import router as search_router
# TODO (P4): Import the search router

app = FastAPI(title="Dron Logistics API - Polyglot Edition", version="0.4.0")

app.include_router(drons_router)
app.include_router(missions_router)
app.include_router(search_router)
# TODO (P4): Include the search router

@app.get("/")
def index():
    return {"name": app.title, "version": app.version}
