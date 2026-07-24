import fastapi
from .drons.controller import router as dronsRouter

app = fastapi.FastAPI(title="Dron Logistics API", version="0.1.0")

app.include_router(dronsRouter)

@app.get("/")
def index():
    return {"name": app.title, "version": app.version}
