import pytest
from fastapi.testclient import TestClient
from datetime import datetime, timedelta
from app.main import app

client = TestClient(app)

def test_create_telemetria():
    response = client.post(
        "/telemetria/",
        json={
            "dron_id": 1,
            "latitud": 41.3851,
            "longitud": 2.1734,
            "altitud": 100.0,
            "bateria": 85.5,
            "velocidad_viento": 12.0,
            "temperatura_motor": 45.0
        },
    )
    assert response.status_code == 200
    data = response.json()
    assert data["dron_id"] == 1
    assert "timestamp" in data

def test_get_telemetria_by_dron():
    # Insert multiple points
    for i in range(5):
        client.post(
            "/telemetria/",
            json={
                "dron_id": 2,
                "latitud": 41.0 + i,
                "longitud": 2.0 + i,
                "altitud": 100.0,
                "bateria": 80.0,
                "velocidad_viento": 10.0,
                "temperatura_motor": 40.0
            },
        )
    
    response = client.get("/telemetria/2")
    assert response.status_code == 200
    data = response.json()
    assert len(data) >= 5

def test_get_telemetria_with_timerange():
    dron_id = 3
    # Point 1 (Old)
    # We can't easily mock the timestamp since it's generated in the repo, 
    # but we can sleep or just check the logic with current time.
    client.post("/telemetria/", json={"dron_id": dron_id, "latitud": 1, "longitud": 1, "altitud": 1, "bateria": 1, "velocidad_viento": 1, "temperatura_motor": 1})
    
    start_time = datetime.now().isoformat()
    
    # Point 2 (New)
    client.post("/telemetria/", json={"dron_id": dron_id, "latitud": 2, "longitud": 2, "altitud": 2, "bateria": 2, "velocidad_viento": 2, "temperatura_motor": 2})
    
    response = client.get(f"/telemetria/{dron_id}?start={start_time}")
    assert response.status_code == 200
    data = response.json()
    assert len(data) == 1
    assert data[0]["latitud"] == 2.0

def test_get_stats():
    dron_id = 4
    client.post("/telemetria/", json={"dron_id": dron_id, "latitud": 0, "longitud": 0, "altitud": 100, "bateria": 50, "velocidad_viento": 10, "temperatura_motor": 0})
    client.post("/telemetria/", json={"dron_id": dron_id, "latitud": 0, "longitud": 0, "altitud": 200, "bateria": 40, "velocidad_viento": 20, "temperatura_motor": 0})
    
    response = client.get(f"/telemetria/stats/{dron_id}")
    assert response.status_code == 200
    data = response.json()
    assert data["avg_bateria"] == 45.0
    assert data["max_altitud"] == 200.0
    assert data["avg_viento"] == 15.0

def test_tunable_consistency():
    """Verifica que el paràmetre de consistència es processa correctament"""
    response = client.post(
        "/telemetria/?consistency=quorum",
        json={
            "dron_id": 5,
            "latitud": 41.0,
            "longitud": 2.0,
            "altitud": 10.0,
            "bateria": 100.0,
            "velocidad_viento": 0.0,
            "temperatura_motor": 20.0
        },
    )
    assert response.status_code == 200
