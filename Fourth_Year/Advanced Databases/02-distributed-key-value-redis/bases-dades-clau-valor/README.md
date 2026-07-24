[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/eFcJbdqb)
# Logística de Drons - P2: Sistemes de Clau-Valor distribuïts amb Redis

## Context Professional

L'empresa de missatgeria amb drons s'enfronta a un escenari de creixement hiper-escalat. La persistència relacional pura (PostgreSQL) ha demostrat ser un coll d'ampolla per a dades d'alta freqüència i consultes geogràfiques complexes. 

**El teu rol:** Com a arquitecte de dades, has de redissenyar el cor del sistema per implementar una arquitectura de persistència políglota, delegant la telemetria i el rànquing operacional a **Redis**.

---

## Repte d'Arquitectura i Consistència

En aquesta pràctica, no només "usaràs Redis"; has de garantir la **consistència eventual i la integritat referencial** entre dos motors de base de dades diferents.

### Objectius Tècnics

1.  **Estat Operacional (Hashes)**: Gestiona la telemetria (bateria, estat) en memòria per a respostes de baixa latència.
2.  **Rànquings en Temps Real (Sorted Sets)**: Implementa la lògica de rànquings d'eficiència utilitzant estructures de dades natives de Redis.
3.  **Indexació Espacial de Proximitat (GEO)**: Resol consultes de discovery geogràfic sense penalitzar la base de dades relacional.

---

## Desenvolupament de l'API

L'API ha estat ampliada. La teva implementació al `repository.py` serà avaluada sota criteris de rigor d'enginyeria:

- **Estratègia**: Com combines dades estàtiques (Postgres) i dinàmiques (Redis) de forma eficient?
- **Neteja i Higiene**: Qualsevol anomalia (drons fantasma a Redis que no existeixen a Postgres) serà considerada un error greu de disseny.
- **Eficiència de Codi**: Evita crides redundants a la xarxa.

---

## Observabilitat i Rendiment

Un enginyer de dades ha de monitoritzar el sistema mentre es desenvolupa. 
- **Accés**: http://localhost:3000
- **Dashboards Crítics**:
    - `Redis Performance Dashboard`: Monitoritza la memòria i el throughput durant l'execució dels tests.

---

## Instruccions Crítiques

1.  **Autonomia**: El `repository.py` conté les signatures i els requisits, però la lògica d'integració és totalment teva.
2.  **Validació Rigorosa**: A més del test de controlador estàndard, hem inclòs `redis_test.py`. Aquest fitxer verifica directament l'estat intern de Redis per garantir que utilitzes les estructures de dades adequades.

---

## Execució de Proves

```bash
docker exec bdda_api sh -c "pytest"
```

> [!CAUTION]
> Els tests fallaran inicialment. No és un error de l'entorn; és el teu punt de partida.
