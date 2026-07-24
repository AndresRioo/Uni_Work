from fastapi.testclient import TestClient
import pytest
from app.main import app
import uuid

client = TestClient(app)

@pytest.fixture(scope="session", autouse=True)
def clear_db():
    from app.database import SessionLocal, engine
    from app.drons import models
    models.Base.metadata.drop_all(bind=engine)
    models.Base.metadata.create_all(bind=engine)

def test_create_dron():
    """A dron can be properly created"""
    response = client.post("/drons", json={"name": "Dron 1", "latitude": 1.0, "longitude": 1.0})
    assert response.status_code == 200
    json = response.json()
    assert "id" in json
    assert uuid.UUID(json["id"])
    assert json["name"] == "Dron 1"

def test_get_dron_by_id():
    """We can get a dron by its id"""
    # Create another one to get its ID
    create_res = client.post("/drons", json={"name": "Dron 2", "latitude": 2.0, "longitude": 2.0})
    dron_id = create_res.json()["id"]
    
    response = client.get(f"/drons/{dron_id}")
    assert response.status_code == 200
    assert response.json()["name"] == "Dron 2"

def test_get_all_drons():
    """🙋🏽‍♀️ First assignment: make this test pass, it should return a list with the drons created previously"""
    response = client.get("/drons")
    assert response.status_code == 200
    json = response.json()
    assert isinstance(json, list)
    assert len(json) >= 2

def test_post_dron_data():
    """Sets the data for a dron"""
    create_res = client.post("/drons", json={"name": "Dron 3", "latitude": 3.0, "longitude": 3.0})
    dron_id = create_res.json()["id"]
    
    response = client.post(f"/drons/{dron_id}/data", json={
        "battery_level": 90.5, 
        "last_seen": "2023-01-01T00:00:00.000Z",
        "status": "en_vol"
    })
    assert response.status_code == 200
    json = response.json()
    assert json["battery_level"] == 90.5
    assert json["status"] == "en_vol"

def test_low_battery_error():
    """Verify business logic: cannot fly with low battery"""
    create_res = client.post("/drons", json={"name": "Dron Low Bat", "latitude": 0.0, "longitude": 0.0})
    dron_id = create_res.json()["id"]
    
    # First, let's actually set it to low battery in a valid state (disponible/bateria_baixa)
    client.post(f"/drons/{dron_id}/data", json={
        "battery_level": 15.0, 
        "last_seen": "2023-01-01T00:00:00.000Z",
        "status": "bateria_baixa"
    })
    
    # Now try to set it to en_vol, which should fail
    response = client.post(f"/drons/{dron_id}/data", json={
        "battery_level": 15.0, 
        "last_seen": "2023-01-01T00:00:00.000Z",
        "status": "en_vol"
    })
    assert response.status_code == 400
    assert "Cannot fly with battery below 20%" in response.json()["detail"]

def test_delete_dron():
    """Deletes a dron"""
    create_res = client.post("/drons", json={"name": "Dron to Delete", "latitude": 0.0, "longitude": 0.0})
    dron_id = create_res.json()["id"]
    
    response = client.delete(f"/drons/{dron_id}")
    assert response.status_code == 200
    
    get_res = client.get(f"/drons/{dron_id}")
    assert get_res.status_code == 404