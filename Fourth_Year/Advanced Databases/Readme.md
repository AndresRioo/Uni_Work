# Advanced Databases – Polyglot Persistence Architecture

Este repositorio reúne las prácticas desarrolladas durante la asignatura **Advanced Databases (BDDA)**. A lo largo de seis proyectos consecutivos se diseña y evoluciona una plataforma de gestión logística para una flota de drones, incorporando progresivamente diferentes paradigmas de almacenamiento y procesamiento de datos.

El objetivo principal es comprender cuándo y por qué utilizar cada tecnología, construyendo una arquitectura de persistencia políglota donde cada componente resuelve un problema específico de forma eficiente.

## Arquitectura Evolutiva

| Proyecto                                   | Tecnología Principal | Paradigma             | Objetivo                                              |
| ------------------------------------------ | -------------------- | --------------------- | ----------------------------------------------------- |
| `01-relational-databases-postgresql`       | PostgreSQL           | Relacional            | Gestión de entidades y persistencia transaccional     |
| `02-distributed-key-value-redis`           | Redis                | Key-Value Distribuido | Telemetría en tiempo real, rankings y geolocalización |
| `03-document-databases-mongodb`            | MongoDB              | Documental            | Almacenamiento flexible de misiones y eventos         |
| `04-search-engines-elasticsearch`          | Elasticsearch        | Motor de Búsqueda     | Búsquedas avanzadas y análisis forense                |
| `05-wide-column-databases-cassandra`       | Apache Cassandra     | Wide-Column           | Ingesta masiva de series temporales                   |
| `06-message-driven-architectures-rabbitmq` | RabbitMQ             | Mensajería Asíncrona  | Desacoplamiento y procesamiento distribuido           |

---

# 01 · Relational Databases with PostgreSQL

## Objetivo

Construir la base del sistema mediante una arquitectura relacional tradicional, implementando la gestión de drones y sus operaciones básicas.

Esta práctica introduce conceptos fundamentales como:

* Modelado relacional
* Diseño de esquemas SQL
* CRUD APIs
* Identificadores UUID
* Persistencia transaccional
* Integración entre FastAPI y PostgreSQL

## Tecnologías

* PostgreSQL
* FastAPI
* SQLAlchemy
* Docker
* Grafana

## Resultado

Se implementa una API REST capaz de:

* Registrar drones
* Consultar información de la flota
* Gestionar estados operativos
* Persistir información de forma consistente

---

# 02 · Distributed Key-Value Systems with Redis

## Objetivo

Resolver problemas de baja latencia y alta frecuencia de escritura utilizando Redis como sistema complementario a PostgreSQL.

Se introducen:

* Hashes
* Sorted Sets
* GEO Indexes
* Cache distribuida
* Persistencia políglota

## Tecnologías

* Redis
* PostgreSQL
* FastAPI
* Docker
* Grafana

## Resultado

El sistema incorpora:

* Telemetría en tiempo real
* Rankings de eficiencia
* Consultas geoespaciales
* Gestión híbrida SQL + Redis

---

# 03 · Document Databases with MongoDB

## Objetivo

Gestionar información semi-estructurada mediante un modelo documental flexible.

Se aborda:

* Diseño schema-less
* Programación asíncrona
* Persistencia documental
* Logs heterogéneos
* Históricos de misiones

## Tecnologías

* MongoDB
* Motor (Async MongoDB Driver)
* FastAPI
* Pydantic

## Resultado

Se desarrolla un módulo completo de gestión de misiones capaz de almacenar:

* Eventos de vuelo
* Entregas
* Inspecciones
* Alertas
* Telemetría especializada

sin necesidad de esquemas rígidos.

---

# 04 · Search Engines with Elasticsearch

## Objetivo

Implementar capacidades avanzadas de búsqueda y análisis sobre millones de registros históricos.

Conceptos trabajados:

* Full-text search
* Query DSL
* Fuzzy Search
* Highlighting
* Agregaciones
* Indexación especializada

## Tecnologías

* Elasticsearch
* Kibana
* FastAPI

## Resultado

Se construye un sistema de análisis capaz de:

* Detectar incidencias
* Buscar patrones complejos
* Corregir errores tipográficos
* Generar estadísticas en tiempo real

---

# 05 · Wide-Column Databases with Apache Cassandra

## Objetivo

Diseñar una solución capaz de soportar cargas masivas de escritura para telemetría IoT.

Conceptos principales:

* Wide-Column Stores
* Partition Keys
* Clustering Columns
* Consistency Levels
* Time Series Data
* Query-Driven Modeling

## Tecnologías

* Apache Cassandra
* FastAPI
* Docker
* Grafana

## Resultado

El sistema pasa a gestionar:

* Millones de eventos de telemetría
* Históricos temporales
* Consultas optimizadas por rango temporal
* Escalabilidad horizontal

---

# 06 · Message-Driven Architectures with RabbitMQ

## Objetivo

Desacoplar productores y consumidores mediante una arquitectura basada en eventos y colas de mensajes.

Conceptos trabajados:

* Event-Driven Architecture
* AMQP
* Publishers
* Consumers
* Exchanges
* Routing Keys
* Dead Letter Queues
* Procesamiento asíncrono

## Tecnologías

* RabbitMQ
* aio-pika
* Cassandra
* Redis
* PostgreSQL
* FastAPI

## Resultado

La plataforma evoluciona hacia una arquitectura distribuida donde:

1. La API publica eventos.
2. RabbitMQ distribuye los mensajes.
3. Workers independientes procesan la información.
4. Los datos se almacenan en distintos motores especializados.

Esto permite aumentar el rendimiento, la resiliencia y la escalabilidad del sistema.

---

# Tecnologías Utilizadas

* PostgreSQL
* Redis
* MongoDB
* Elasticsearch
* Apache Cassandra
* RabbitMQ
* FastAPI
* SQLAlchemy
* Docker
* Grafana
* Kibana

