import pytest
import uuid
import asyncio
from httpx import AsyncClient
from app.main import app

@pytest.fixture
async def async_client():
    async with AsyncClient(app=app, base_url="http://test") as ac:
        yield ac

@pytest.mark.anyio
async def test_init_index(async_client):
    """Step 1: Create index with mappings"""
    response = await async_client.post("/search/init-index")
    assert response.status_code == 200

@pytest.mark.anyio
async def test_index_and_search_fuzzy(async_client):
    """Step 2 & 3: Indexing and Fuzzy Search"""
    dron_id = str(uuid.uuid4())
    report = {
        "dron_id": dron_id,
        "severity": "high",
        "component": "motor",
        "description": "The rear motor suffered a short circuit during takeoff",
        "pilot_notes": "Smell of burnt plastic"
    }
    
    # Index
    await async_client.post("/search/index", json=report)
    
    # Wait for refresh (ES is near real-time)
    import asyncio
    await asyncio.sleep(1)
    
    # Search 'circut' (misspelled) with fuzzy=True
    response = await async_client.post("/search/query", json={
        "text": "circut",
        "fuzzy": True
    })
    assert response.status_code == 200
    results = response.json()
    assert len(results) > 0
    assert "short circuit" in results[0]["description"]

@pytest.mark.anyio
async def test_aggregations(async_client):
    """Step 4: Aggregations by severity"""
    # Index multiple reports
    reports = [
        {"dron_id": str(uuid.uuid4()), "severity": "low", "component": "gps", "description": "minor lag"},
        {"dron_id": str(uuid.uuid4()), "severity": "high", "component": "battery", "description": "overheating"},
        {"dron_id": str(uuid.uuid4()), "severity": "high", "component": "motor", "description": "vibration"}
    ]
    for r in reports:
        await async_client.post("/search/index", json=r)
    
    await asyncio.sleep(1)
    
    response = await async_client.get("/search/stats")
    assert response.status_code == 200
    stats = response.json()
    # Expect counts for high (at least 2) and low (at least 1)
    assert stats.get("high", 0) >= 2
    assert stats.get("low", 0) >= 1
