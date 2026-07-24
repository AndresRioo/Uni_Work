from fastapi import APIRouter, Depends, HTTPException
from .models import IncidentReport, SearchQuery
from .repository import SearchRepository
from .dependencies import get_elasticsearch
from elasticsearch import AsyncElasticsearch

router = APIRouter(prefix="/search", tags=["Forensic Search"])

async def get_repository(es: AsyncElasticsearch = Depends(get_elasticsearch)):
    return SearchRepository(es)

@router.post("/index")
async def index_report(report: IncidentReport, repo: SearchRepository = Depends(get_repository)):
    await repo.index_incident_report(report)
    return {"status": "indexed"}

@router.post("/query")
async def search_incidents(query: SearchQuery, repo: SearchRepository = Depends(get_repository)):
    results = await repo.search_incidents(query)
    return results

@router.get("/stats")
async def get_stats(repo: SearchRepository = Depends(get_repository)):
    return await repo.get_severity_stats()

@router.post("/init-index")
async def init_index(repo: SearchRepository = Depends(get_repository)):
    await repo.create_index()
    return {"status": "index created"}
