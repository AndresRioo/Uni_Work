# Logística de Drons - P1

Benvingut a la plataforma de gestió de flota de l'empresa de missatgeria amb drons. Ets l'arquitecte de dades encarregat de dissenyar un sistema robust per gestionar una flota de drons que reparteix paquets per tota la ciutat.

Aquesta és la P1 d'una sèrie de pràctiques on evolucionaràs aquest sistema cap a una arquitectura de persistència políglota i escalable.

## Què conté la plantilla?

La plantilla conté una API REST que et permetrà gestionar els drons, registrar la seva telemetria i monitoritzar el seu estat. Està implementada amb **FastAPI** i connectada a una base de dades **PostgreSQL**.

A més, l'entorn ja inclou eines d'observabilitat:
- **FastAPI**: L'API principal (port 8000).
- **PostgreSQL**: La base de dades relacional on guardem la informació.
- **Grafana**: Per visualitzar l'estat de la flota en temps real (port 3000).

## Requisits previs

Necessitaràs tenir instal·lat **Docker** i **Docker Compose**.

- [Instal·lar Docker](https://docs.docker.com/get-docker/)
- [Instal·lar Docker Compose](https://docs.docker.com/compose/install/)

## Com començar?

1. Clona el repositori.
2. Arranca l'entorn amb:
   ```bash
   docker-compose up
   ```

Això aixecarà l'API, la base de dades i el Dashboard de Grafana.

## Visualització i Monitorització

Com a arquitecte, necessites veure què està passant. Hem configurat un dashboard de **Grafana** perquè puguis monitoritzar la flota:

- **Accés**: http://localhost:3000
- **Usuari**: admin / admin (o accés anònim preconfigurat)
- **Dashboard**: "Drone Logistics Dashboard"

Des d'aquí podràs veure el llistat de drons, la seva bateria i el seu estat actual (`disponible`, `en_vol`, `retonant`, etc.).

## Domini de Dades: El Dron

L'entitat principal és el `Dron`. A diferència de sistemes simples, el nostre ús d'identificadors es basa en **UUIDs** per garantir la unicitat i seguretat en sistemes distribuïts.

Camps de l'entitat `Dron`:
- `id`: UUID únic del dron.
- `name`: Nom descriptiu (ex: "Falcon-1").
- `status`: Estat actual (`disponible`, `en_vol`, `retornant`, `bateria_baixa`, `manteniment`).
- `battery_level`: Percentatge de bateria (0-100).
- `latitude` / `longitude`: Ubicació actual.
- `last_seen`: Última vegada que el dron va enviar telemetria.

## Rutes de l'API

L'API es troba a http://localhost:8000 i pots consultar la documentació Interactiva a `/docs`.

Rutes principals:
- `GET /drons`: Retorna tots els drons.
- `GET /drons/{id}`: Retorna el dron amb el seu UUID.
- `POST /drons`: Registra un nou dron.
- `DELETE /drons/{id}`: Elimina un dron del sistema.
- `POST /drons/{id}/data`: Registra telemetria (estat, bateria, etc.).

## Què hem de fer?

Cada pràctica tindrà com a objectiu afegir noves funcionalitats i connexions a l'aplicació, a cada una trobaràs un fitxer README amb les instruccions de la pràctica i una serie de tests automàtics que has de passar per poder-la entregar.

L'objectiu d'aquesta primera entrega és que et familiaritzis amb el funcionament de FastAPI i amb la creació de rutes. És a dir, no té un objectiu concret. Només has de seguir els passos que s'indiquen a continuació.

### Punt 1: Provar els tests

Per començar, has de provar els tests que s'han creat per aquesta pràctica. Per a fer-ho, has de fer servir el següent comandament:

```bash
docker exec bdda_api sh -c "pytest"
```

Veurem que part dels tests fallen. Això és normal, ja que l'objectiu de les pràctiques és fer passar tots els tests.

### Punt 2: Crear una ruta GET per a obtenir tots els drons

La primera ruta que s'ha de crear és una ruta GET que ens permeti obtenir tots els drons de la base de dades. Per això, has de crear una ruta GET a la ruta `/drons` que retorni tots els drons de la base de dades.

Tornem a executar el servidor i fem una petició GET a http://localhost:8000/drons per a veure si funciona correctament. Si tot ha anat bé, hauríem de rebre els drons que hem creat.


### Punt 3: Fer servir la ruta `POST /drons` per a crear un dron

La ruta per crear un dron és la ruta `POST /drons`. Aquesta ruta ja està implementada, intenta trobar el mètode al codi i veuràs que rep un objecte JSON amb els següents camps:

- `name`: Nom del dron.
- `latitude`: Coordenada de latitud del dron.
- `longitude`: Coordenada de longitud del dron.

I si la petició és vàlida, crea un nou dron a la base de dades i retorna el dron creat.

Per provar aquesta ruta, pots fer servir el següent comandament:

```bash
curl -X POST -H "Content-Type: application/json" -d '{"name": "Dron 1", "latitude": 41.387917, "longitude": 2.169919}' http://localhost:8000/drons
```

### Punt 4: Clients de dades i APIs

Tot i que pots fer servir `curl` per fer peticions a l'API, és molt més recomanable fer servir un client de dades modern. Aquests clients et permeten guardar les peticions, organitzar-les en col·leccions i veure les respostes de forma molt més clara.

Et recomanem fer servir algun d'aquests:
- **[Bruno](https://www.usebruno.com/)**: Un client de codi obert, lleuger i molt potent (millor opció).
- **[Postman](https://www.postman.com/)**: El client més popular i utilitzat.
- **[Insomnia](https://insomnia.rest/)**: Una alternativa molt polida i fàcil de fer servir.

Al repositori de la pràctica trobaràs una col·lecció de peticions ja configurada perquè puguis començar a treballar ràpidament.

### Punt 5: Executar els tests

Ara que ja has implementat les rutes, pots tornar a executar els tests per a veure si has fet bé les coses. Per fer-ho, has de fer servir el següent comandament:

```bash
docker exec bdda_api sh -c "pytest"
```

Si tot ha anat bé, hauries de veure que tots els tests passen.

## Entrega

Durant les pràctiques farem servir GitHub Classroom per gestionar les entregues. Per tal de clonar el repositori ja has hagut d'acceptar l'assignment a GitHub Classroom. Per entregar la pràctica has de pujar els canvis al teu repositori de GitHub Classroom. El repositori conté els tests i s'executa automàticament cada vegada que puges els canvis al teu repositori. Si tots els tests passen, la pràctica està correctament entregada.

Per pujar els canvis al teu repositori, has de fer servir el següent comandament:

```bash
git add .
git commit -m "Canvis per a la pràctica 1"
git push
```

## Puntuació

Aquesta primera pràctica no té puntuació. Només has de fer-la per familiaritzar-te amb el funcionament de FastAPI i amb la creació de rutes i connexions amb una base de dades.

## Qüestionari d'avaluació de cada pràctica

Cada pràctica té un qüestionari d'avaluació. Aquest qüestionari ens permet avaluar el coneixement teòric i de comprensió de la pràctica. És obligatori i forma part de l'avaluació continua de l'assignatura.


