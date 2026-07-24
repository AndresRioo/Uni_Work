import pytest
import uuid
import asyncio
from httpx import AsyncClient
from app.main import app
from app.mongodb_client import get_mongodb

# Anyio is used here for better compatibility with different environments
@pytest.fixture
async def async_client():
    async with AsyncClient(app=app, base_url="http://test") as ac:
        yield ac

@pytest.fixture(autouse=True)
async def clean_mongo():
    db = get_mongodb()
    await db["missions"].delete_many({})

@pytest.mark.anyio
async def test_missions_api_registered(async_client):
    """Verify that the student has registered the /missions router"""
    response = await async_client.get("/missions/search?q=test")
    assert response.status_code != 404, "The /missions router is not registered in main.py!"

@pytest.mark.anyio
async def test_create_and_get_mission(async_client):
    """Verify mission creation in MongoDB"""
    dron_id = str(uuid.uuid4())
    mission_data = {
        "dron_id": dron_id,
        "payload_type": "camera",
        "mission_objective": "Surveillance of Sector 5",
        "start_time": "2023-01-01T10:00:00Z"
    }
    
    # Create
    response = await async_client.post("/missions", json=mission_data)
    assert response.status_code == 200
    mission_id = response.json()
    assert isinstance(mission_id, str)
    
    # Get by Dron
    response = await async_client.get(f"/missions/dron/{dron_id}")
    assert response.status_code == 200
    data = response.json()
    assert len(data) == 1
    assert data[0]["mission_objective"] == "Surveillance of Sector 5"
    assert "logs" in data[0]
    assert len(data[0]["logs"]) == 0

@pytest.mark.anyio
async def test_add_mission_events(async_client):
    """Verify that events are properly pushed to the logs array"""
    dron_id = str(uuid.uuid4())
    create_res = await async_client.post("/missions", json={
        "dron_id": dron_id,
        "payload_type": "sensor",
        "mission_objective": "Environmental sensing"
    })
    mission_id = create_res.json()
    
    # Add Event 1
    event1 = {
        "event_type": "takeoff",
        "details": {"altitude": 10, "battery": 100}
    }
    res1 = await async_client.post(f"/missions/{mission_id}/events", json=event1)
    assert res1.status_code == 200
    
    # Add Event 2 (Different structure)
    event2 = {
        "event_type": "sensor_reading",
        "details": {"temp": 22.5, "humidity": 60, "co2": 400}
    }
    res2 = await async_client.post(f"/missions/{mission_id}/events", json=event2)
    assert res2.status_code == 200
    
    # Verify Logs
    response = await async_client.get(f"/missions/dron/{dron_id}")
    mission = response.json()[0]
    assert len(mission["logs"]) == 2
    assert mission["logs"][0]["event_type"] == "takeoff"
    assert mission["logs"][1]["details"]["temp"] == 22.5

@pytest.mark.anyio
async def test_search_missions(async_client):
    """Verify discovery of missions by objective keyword"""
    await async_client.post("/missions", json={
        "dron_id": str(uuid.uuid4()),
        "payload_type": "package",
        "mission_objective": "Deliver medicine to hospital"
    })
    await async_client.post("/missions", json={
        "dron_id": str(uuid.uuid4()),
        "payload_type": "package",
        "mission_objective": "Deliver food to shelter"
    })
    
    # Search 'medicine'
    response = await async_client.get("/missions/search?q=medicine")
    assert response.status_code == 200
    assert len(response.json()) == 1
    assert "hospital" in response.json()[0]["mission_objective"]
    
    # Search 'deliver' (both)
    response = await async_client.get("/missions/search?q=Deliver")
    assert response.status_code == 200
    assert len(response.json()) == 2
