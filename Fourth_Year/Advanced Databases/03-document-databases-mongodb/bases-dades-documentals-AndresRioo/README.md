[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/SChb8oab)
# Logística de Drons - P3: Bases de Dades Orientades a Documents amb MongoDB

## El Caos del Món Real: Missions i Telemetria Complexa

Fins ara, el nostre sistema gestiona la identitat dels drons (Postgres) i el seu estat de bateria i rànquing (Redis). Però l'empresa ha evolucionat: ara els drons no només volen, sinó que executen **Missions**.

Cada missió és un món: unes porten paquets i registren el pes i el destinatari; altres fan vigilància i registren alertes de seguretat; unes altres fan inspeccions tècniques amb milers de dades de sensors de vibració. Intentar guardar tot això en una taula SQL és un malson de columnes buides, i guardar-ho a Redis és ineficient per a consultes històriques complexes.

**La teva missió:** Implementar un servei de **Mission Control** utilitzant **MongoDB**. Aprofitaràs la naturalesa esquema-lliure (schema-less) de MongoDB per guardar logs de missions semi-estructurats que canvien segons el tipus de vol.

---

## Evolució de l'Arquitectura Políglota

A partir d'ara, el teu sistema és una bèstia de 3 caps:
1. **PostgreSQL**: Persistència relacional per a la identitat i metadades mestre dels drons.
2. **Redis**: Cache i rànquing operacional per a dades d'alta freqüència.
3. **MongoDB**: Magatzem de documents per al log detallat i heterogeni de les missions.

---

## El Desafiament d'Enginyeria

Aquesta pràctica t'exigeix dominar una nova competència: la **programació asíncrona**.

1.  **Driver Asíncron (Motor)**: No utilitzaràs el driver estàndard de Python. Has d'utilitzar `Motor`, el driver asíncron de MongoDB per a Tornado/FastAPI. Això vol dir que el teu repositori estarà ple d' `async` i `await`.
2.  **Semi-estructura i Logs**: Una Missió no té un format fix. Al camp `logs` (una llista de documents), cada esdeveniment pot tenir camps totalment diferents (un 'takeoff' té altitud, un 'package_delivered' té signatura digital).
3.  **Integració de Serveis**: Hauràs de crear el mòdul `missions` des de zero, definir els esquemes de Pydantic i registrar el router a l'aplicació principal.

---

## Rutes de l'API de Missions

- `POST /missions`: Crear una nova missió per a un dron.
- `GET /missions/dron/{dron_id}`: Historial de missions d'un dron específic.
- `POST /missions/{mission_id}/events`: Afegir un esdeveniment al log de la missió. **REQUISIT**: Has d'utilitzar l'operador `$push` de MongoDB per garantir atomicitat.
- `GET /missions/search?q=keyword`: Cerca de missions per paraula clau en l'objectiu de la missió.

---

## Observabilitat en un Sistema Distribuït

Amb tres bases de dades, la monitorització és vital:
- **Dashboards**: Ara tens tres visuals:
    1. Estat de la Flota (SQL)
    2. Rendiment de Redis (K-V)
    3. **Rendiment de MongoDB** (Documental) - Mira les operacions per segon i la memòria resident.

---

## Instruccions per a l'Entrega

1.  **Sincronització**: Recorda que les pràctiques són acumulatives. Les rutes de la P2 (Redis) segueixen sent part de l'avaluació!
2.  **Qualitat de Codi**: Un bon enginyer ha d'escriure codi net. No barregis lògica de base de dades al controlador.
3.  **Proves**: Hem inclòs `missions_test.py`. Aquest test verifica la persistència asíncrona i l'estructura de documents.

---

## Com executar els tests?

```bash
docker exec bdda_api sh -c "pytest"
```
