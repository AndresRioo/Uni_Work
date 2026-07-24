[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XDIY__ch)
# Logística de Drons - P5: Arquitectures Basades en Missatgeria (RabbitMQ)

Hem arribat a la darrera pràctica. La nostra flota de drons no només envia telemetria, sinó que hem activat un sistema d'**Alertes Crítiques** i **Processament en Temps Real**. El flux de dades és tan intens que l'API ja no pot esperar a que les bases de dades (Cassandra, Postgres, Redis) responguin per confirmar que la dada s'ha guardat.

Si Cassandra triga 50ms a confirmar una escriptura i tenim milers de peticions simultànies, les connexions d'usuari es queden bloquejades esperant el disc. Això és el que anomenem **acoblament fort**.

### La Solució: Desacoblament amb RabbitMQ

En aquesta pràctica introduirem una capa de **Messaging Broker** amb RabbitMQ. L'arquitectura canvia radicalment:

1.  **L'API (Productor)**: Rep la telemetria, la valida mínimament i la llança a una **cua** de RabbitMQ. Immediatament respon al dron amb un codi `202 Accepted`. L'API ja no sap (ni li importa) quan es guardarà la dada a la base de dades.
2.  **El Worker (Consumidor)**: Un procés independent que corre en segon pla, llegeix els missatges de la cua a la velocitat que pot i els processa:
    *   Guarda a **Cassandra** (l'històric).
    *   Actualitza la posició a **Redis** (per al mapa en temps real).
    *   Verifica si hi ha alertes (per exemple, bateria < 10%) i les envia a PostgreSQL.
---

## Conceptes de Missatgeria i RabbitMQ

### El model AMQP
RabbitMQ no és només una "cua". És un sistema flexible basat en:
- **Publishers**: Qui envia missatges (la nostra API).
- **Exchanges**: L' "enrutador". Rep els missatges i decideix a quina cua van segons les regles (**Bindings**).
- **Queues**: On s'emmagatzemen els missatges fins que algú els llegeix.
- **Consumers**: Qui processa la feina (el nostre Worker).

### Tipus de Exchanges
En aquesta pràctica utilitzarem un exchange de tipus **Topic**. Això ens permet enrutaments complexos com:
- `drone.123.telemetria`
- `drone.alerts.critical`
Podem subscriure'ns a `drone.*.telemetria` per rebre-ho tot!

### Dead Letter Exchanges (DLX)
Què passa si un missatge falla repetidament (per exemple, Cassandra està caigut)? 
No podem perdre la dada. RabbitMQ ens permet configurar una **Dead Letter Queue (DLQ)** on els missatges "morts" s'emmagatzemen per a una revisió posterior o reintent automàtic.

---

## Implementació del Flux Asíncron

En aquesta pràctica, hauràs de completar el circuit de missatgeria.

### Tasques principals

1.  **Configuració del Client de RabbitMQ**: Implementar la connexió asíncrona utilitzant `aio-pika`.
2.  **El Productor (API)**: Modificar l'endpoint de telemetria per a que publiqui missatges en lloc d'escriure directament a la base de dades.
3.  **El Consumidor (Worker)**: Implementar la lògica del worker que consumeix de la cua `telemetria_queue`.
4.  **Gestió d'Errors (DLX)**: Configurar la cua per a que els missatges fallits vagin a `telemetria_errors`.

---

## Observabilitat Avançada

A més de Grafana, ara tens accés a la **RabbitMQ Management Console**:
- **URL**: `http://localhost:15672`
- **Usuari/Password**: `guest` / `guest`

Des d'aquí podràs veure gràfiques en temps real de quants missatges hi ha a les cues i la velocitat de processament del teu worker.

---

## Instruccions per a l'Entrega

1.  **Docker Compose**: Ara s'aixequen dos containers de codi: `api` i `worker`.
2.  **Tests**: Els tests ara verifiquen que l'API respon ràpid i que, passat un segon, la dada apareix a Cassandra.
```bash
docker exec bdda_api sh -c "pytest"
```