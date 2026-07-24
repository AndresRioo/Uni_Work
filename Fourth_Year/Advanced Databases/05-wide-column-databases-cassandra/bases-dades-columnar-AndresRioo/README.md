[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/60a6Jycr)
# Logística de Drons - P4: Bases de Dades Wide-Column amb Apache Cassandra

## El repte de la Telemetria Massiva

La nostra flota de drons ha crescut fins a les **10.000 unitats** en actiu. Cada dron envia dades de telemetria (GPS, bateria, sensors) cada **200 mil·lisegons**. Això genera milions de files per hora. 

En les pràctiques anteriors hem après a utilitzar models relacionals (PostgreSQL), caches de clau-valor (Redis) i magatzems de documents (MongoDB). Però, què passa quan la velocitat d'escriptura és tan alta que cap d'aquestes bases de dades pot seguir el ritme sense col·lapsar?

### El Dilema de l'Arquitectura: Per què Cassandra?

Fins ara, hem estat "víctimes del nostre propi èxit". La nostra infraestructura anterior ha arribat al límit:

1.  **L'Adéu a Elasticsearch**: Tot i ser fantàstic per a cerques textuals forenses, el consum de RAM pels índexs de telemetria pura era inasumible. Hem prioritzat l'estabilitat del sistema eliminant aquest pes per a la càrrega de dades en brut.
2.  **La Substitució de MongoDB**: MongoDB és excel·lent per a documents flexibles, però sota una pressió d'escriptura massiva de sèries temporals, Cassandra ofereix una eficiència superior i una arquitectura "Masterless" (sense node mestre) que li permet escalar linealment en diferents nodes distribuïts sense colls d'ampolla.
3.  **Wide-Column Store**: A diferència de PostgreSQL (orientada a files), Cassandra utilitza una estructura **LSM-Tree** que fa les escriptures extremadament ràpides al escriure primer a memòria i després fer flush a disc de manera seqüencial.

---

## Teoria: Què és una Base de Dades Wide-Column?

### Clarificació Important

Cassandra és una base de dades **wide-column store**, NO una base de dades "columnar" (column-oriented) com ClickHouse o Parquet.

- **Wide-column (Cassandra)**: Optimitzada per **escriptures massives** (IoT, logs, telemetria)
- **Columnar (ClickHouse)**: Optimitzada per **lectures analítiques** (agregacions, data warehouse)

### Conceptes clau de Cassandra

-   **Modelat Query-Driven**: Dissenya les taules segons les **consultes** que necessites, NO segons les entitats. Si vols consultar per "dron_id i temps", la taula ha d'estar físicament ordenada per aquests camps.
-   **Partition Key & Clustering Columns**: La clau primària determina on es guarden les dades (node) i com s'ordenen dins la partició.
-   **Teorema CAP**: Cassandra és **tunable**. Pots configurar el nivell de consistència segons les teves necessitats:
    -   `QUORUM` → Consistència forta (CP)
    -   `ONE` → Alta disponibilitat (AP)

---

## El nou repte: Implementació de Telemetria

En aquesta pràctica, hauràs d'implementar el mòdul de **Telemetria** que recollirà el flux constant de dades dels sensors.

### Tasques principals

1.  **Disseny del model de dades**: Definir la Partition Key i Clustering Columns adequades per les consultes requerides.
2.  **Configuració de consistència**: Decidir els Consistency Levels per escriptures i lectures.
3.  **Implementació de l'API**: Endpoints per ingestió i consulta de telemetria.

---

## Rutes de l'API de Telemetria

- `POST /telemetria/`: Registre d'un nou punt de telemetria.
- `GET /telemetria/{dron_id}`: Historial detallat. Filtra per `start` i `end`.
- `GET /telemetria/stats/{dron_id}`: Estadístiques agregades (bateria mitjana, altitud màxima) calculades pel motor de Cassandra.

---

## Observabilitat

Dashboards actualitzats a Grafana:
1.  **Inventari**: Dades de PostgreSQL.
2.  **Caché**: Rendiment de Redis.
3.  **Cassandra Throughput**: Monitorització d'escriptures per segon i latència.

---

## Instruccions per a l'Entrega

1.  **Esquema Cassandra**: El Keyspace i la Taula s'han de crear automàticament en arrencar l'aplicació (`cassandra_client.py`).
2.  **Tests**: Executa els tests per validar el teu model de dades:
```bash
docker exec bdda_api sh -c "pytest"
```
---
