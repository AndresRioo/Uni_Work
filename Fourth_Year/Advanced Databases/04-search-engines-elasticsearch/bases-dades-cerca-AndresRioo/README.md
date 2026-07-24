[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-DHyWq8R)
# Logística de Drons - P4: Motor de Cerca amb Elasticsearch

## L'Escenari

Amb l'expansió global de la nostra flota, el volum d'informació ha passat de milers a milions de registres. MongoDB és fantàstic per guardar l'historial detallat, però quan un enginyer necessita trobar totes les missions on un dron ha tingut un "possible curtcircuit al motor secundari durant una tempesta a Barcelona", fer cerques per expressió regular a MongoDB es torna insuportablement lent.

Els drons estan començant a patir fallades esporàdiques en components crítics. La teva missió és implementar un sistema de **Forensic Analysis** que permeti als enginyers cercar patrons de text, anomalies i realitzar anàlisis estadístics instantanis sobre tot l'historial d'incidències.

---

## Evolució de l'Arquitectura Políglota

Ara el nostre sistema és una "bèstia de 4 caps":
1. **PostgreSQL**: Inventari mestre i identitat dels drons.
2. **Redis**: Telemetria "viva" i geosensing en temps real.
3. **MongoDB**: Històric massiu d'esdeveniments i logs de missió.
4. **Elasticsearch**: Motor de cerca i analítica per a l'anàlisi forense d'incidències.

---

## Recordatori

Recorda que aquesta pràctica és acumulativa. Perquè el sistema funcioni, hauràs de:
1.  **P1 & P2 (SQL + Redis)**: Implementar el catàleg de drons i la telemetria en temps real a `app/drons/repository.py`.
2.  **P3 (MongoDB)**: Implementar el control de missions i logs a `app/missions/repository.py`.
3.  **P4 (Elasticsearch)**: Implementar la cerca forense a `app/search/repository.py`.
4.  **Integració**: Registrar tots els routers a `app/main.py`.
---

## El nou repte (Elasticsearch)

Aquesta pràctica t'exigeix dominar el **Query DSL** d'Elasticsearch:

1.  **Mappings d'Índex**: Defineix quins camps són `text` (analitzats) i quins són `keyword` (cerques exactes).
2.  **Cerca Difusa (Fuzzy Search)**: Soluciona errors tipogràfics dels pilots (p.ex. "short circut").
3.  **Agregacions**: Calcula estadístiques de errors en temps real.
4.  **Highlighting**: Identifica clarament els acords textuals.

---

## Rutes de l'API de Forensic Search

- `POST /search/init-index`: Crea l'índex amb els mappings configurats.
- `POST /search/index`: Indexa un nou informe d'incidència (`IncidentReport`).
- `POST /search/query`: Cerca avançada amb suport per a filtrat i mode `fuzzy`.
- `GET /search/stats`: Obté agregacions globals sobre les incidències.

---

## Observabilitat i Exploració

- **Kibana**: [http://localhost:5601](http://localhost:5601).
- **Mongo Express**: [http://localhost:8081](http://localhost:8081).
- **Grafana**: [http://localhost:3000](http://localhost:3000).

---

## Com executar els tests?

Per a superar la pràctica, **TOTS** els tests han de passar:

```bash
# Executar tots els tests (SQL, Redis, Mongo, Elasticsearch)
docker exec bdda_api pytest

# Executar només els nous de la P4
docker exec bdda_api pytest app/search/tests/search_test.py
```
