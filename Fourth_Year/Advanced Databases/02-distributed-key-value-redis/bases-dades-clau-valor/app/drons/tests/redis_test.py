from fastapi.testclient import TestClient
import pytest
from app.main import app
import uuid
from app.redis_client import get_redis

client = TestClient(app)
redis = get_redis()

@pytest.fixture(autouse=True)
def clean_redis():
    redis.flushdb()
    yield

def test_redis_telemetry_storage():
    """Verify that telemetry is actually stored in Redis Hashes"""
    create_res = client.post("/drons", json={"name": "Dron Test Redis", "latitude": 41.38, "longitude": 2.17})
    dron_id = create_res.json()["id"]
    
    # Update telemetry
    client.post(f"/drons/{dron_id}/data", json={
        "battery_level": 85.0, 
        "last_seen": "2023-01-01T12:00:00.000Z",
        "status": "en_vol"
    })
    
    # Check Redis Hash
    redis_data = redis.hgetall(f"dron:{dron_id}")
    assert redis_data["status"] == "en_vol"
    assert float(redis_data["battery_level"]) == 85.0

def test_leaderboard_sorted_set():
    """Verify the leaderboard uses Redis Sorted Sets for ranking"""
    drons = [
        {"name": "Dron High", "battery": 95.0},
        {"name": "Dron Mid", "battery": 50.0},
        {"name": "Dron Low", "battery": 10.0}
    ]
    
    for d in drons:
        res = client.post("/drons", json={"name": d["name"], "latitude": 0, "longitude": 0})
        d_id = res.json()["id"]
        client.post(f"/drons/{d_id}/data", json={
            "battery_level": d["battery"],
            "last_seen": "2023-01-01T12:00:00.000Z",
            "status": "disponible"
        })
        
    # Get leaderboard
    lb_res = client.get("/drons/leaderboard?limit=2")
    assert lb_res.status_code == 200
    lb_data = lb_res.json()
    
    assert len(lb_data) == 2
    assert lb_data[0]["name"] == "Dron High"
    assert lb_data[1]["name"] == "Dron Mid"
    
    # Verify Redis ZSET
    zset_count = redis.zcard("drons_battery_leaderboard")
    assert zset_count == 3

def test_geospatial_search():
    """Verify Redis Geospatial indexing and searching"""
    # Dron in Barcelona center
    client.post("/drons", json={"name": "BCN Dron", "latitude": 41.3887, "longitude": 2.1589})
    # Dron in Madrid (far away)
    client.post("/drons", json={"name": "MAD Dron", "latitude": 40.4168, "longitude": -3.7038})
    
    # Search near Barcelona (radius 50km)
    search_res = client.get("/drons/search?latitude=41.38&longitude=2.17&radius=50")
    assert search_res.status_code == 200
    data = search_res.json()
    assert len(data) == 1
    assert data[0]["name"] == "BCN Dron"
    
    # Search with small radius (1km)
    search_res_small = client.get("/drons/search?latitude=41.40&longitude=2.10&radius=1")
    assert len(search_res_small.json()) == 0

def test_deletion_cleanup():
    """Verify that deleting a dron also cleans up all Redis keys"""
    create_res = client.post("/drons", json={"name": "Dron Ghost", "latitude": 0, "longitude": 0})
    dron_id = create_res.json()["id"]
    client.post(f"/drons/{dron_id}/data", json={"battery_level": 50, "last_seen": "2023-01-01T12:00:00.000Z", "status": "en_vol"})
    
    # Ensure it exists in Redis
    assert redis.exists(f"dron:{dron_id}")
    
    # Delete it
    client.delete(f"/drons/{dron_id}")
    
    # Should be gone from Redis
    assert not redis.exists(f"dron:{dron_id}")
    assert redis.zrank("drons_battery_leaderboard", str(dron_id)) is None
    # For GEO it's harder to check directly but ZREM is used under the hood for GEO keys
    # But we can try to search again and it should be empty
    search_res = client.get("/drons/search?latitude=0&longitude=0&radius=10")
    assert len(search_res.json()) == 0
