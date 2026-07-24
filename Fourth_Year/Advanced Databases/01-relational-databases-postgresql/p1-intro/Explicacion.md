

Ejecutar todo :

docker-compose up

Ver como va : 

http://localhost:8000/docs


# API REST

es una forma de exponer funcionalidades de tu backend por HTTP. Sigues convenciones HTTP:

- GET = leer
- POST = crear
- PUT/PATCH = modificar
- DELETE = borrar

Es “REST” porque modelas recursos (drons) y operaciones sobre ellos.

# FastAPI

FastAPI es un framework de Python para crear APIs REST:

- Rutas (@app.get, @router.post, etc.)
- Validación automática de datos (gracias a Pydantic)
- Documentación automática en /docs

# Archivos

## Settings.py

Configuración para leer la bdd y tener su conexión 

## Main.py

Arranca todo, registra el router y define una ruta /

## Database.py

Este archivo monta la conexión con PostgreSQL usando SQLAlchemy.

## Models.py

Modelo ORM. Esto define cómo es la tabla drons en la base de datos.

## Schemas.py

Estos son los modelos Pydantic, lo que entra y sale por la API

`Dron` Lo que pide la api para crear un dron 

``` json 
{
  "name": "Falcon-1",
  "latitude": 41.3,
  "longitude": 2.1
}
```

`DronCreate` : Lo que devuelve la api al pedir un dron

``` json
{
  "id": "...",
  "name": "...",
  "latitude": 41.3,
  "longitude": 2.1,
  "status": "disponible",
  "battery_level": 90
}

```

`DronData` :   Lo que se envía como telemetría

``` json
{
  "battery_level": 50,
  "last_seen": "2026-02-16T10:00:00",
  "status": "en_vol"
}
```

## Repository.py

Aquí está la lógica de acceso a datos + reglas de negocio.

`get_dron` : return db.query(models.Dron).filter(models.Dron.id == dron_id).first()

`get_drons` : POR HACER TO-DO

`create_dron` :

## Controller.py

Define las rutas HTTP

Ejemplo:

``` python
@router.get("", response_model=List[schemas.Dron])
def get_drons(...):

```
Significa:

- Ruta: GET /drons
- Devuelve: lista de Dron
- Internamente llama al repositorio


### Punt 2: Crear una ruta GET per a obtenir tots els drons

en `repository.py` poner

``` python
def get_drons(db: Session, skip: int = 0, limit: int = 100) -> List[models.Dron]:
    return db.query(models.Dron).offset(skip).limit(limit).all()
```

1. La sesion es la base de datos
2. db.query(models.Dron)  : query a la tabla de Drons  (se encuentra en `models.py`)  (`SELECT * FROM drons;`)
3. offset(skip)  (saltar los primeros `skip` registros)
4. limit(limit)  (limitar a un numero maximo)

Puede estar sin eso 

En un sistema real no escala bien.

## DE PYTHONA SQL 

- db.query(models.Dron) : SELECT * FROM TABLA
- db.query(models.Dron).filter(models.Dron.id == dron_id).first() : WHERE id = '...' 
- .all() : ejecutar query


para crear 

``` python 
db_dron = models.Dron(name="Falcon-1", latitude=1.2, longitude=3.4)
db.add(db_dron)
db.commit()
db.refresh(db_dron)
```

## Punt 3: Fer servir la ruta `POST /drons` per a crear un dron

Ya esta implementado en `repositoy.py`

``` python 

def create_dron(db: Session, dron: schemas.DronCreate) -> models.Dron:
    db_dron = models.Dron(name=dron.name, latitude=dron.latitude, longitude=dron.longitude)
    db.add(db_dron)
    db.commit()
    db.refresh(db_dron)
    return db_dron

```

``` powershell

Invoke-RestMethod -Method POST `
  -Uri http://localhost:8000/drons `
  -Headers @{ "Content-Type" = "application/json" } `
  -Body '{"name":"Dron 1","latitude":41.387917,"longitude":2.169919}'

```

## Punt 4: Clients de dades i APIs

és molt més recomanable fer servir un client de dades modern. Aquests clients et permeten guardar les peticions, organitzar-les en col·leccions i veure les respostes de forma molt més clara.

Usar bruno 

Añadir URL

http://localhost:8000/drons

Get - devuelve todos los drones
Post - poner body para crear un dron

{
  "name": "Dron TESTING ",
  "latitude": 41.38,
  "longitude": 2.17
}
