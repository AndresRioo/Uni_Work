# Advanced Databases – Polyglot Persistence Architecture

This repository contains the projects developed during the **Advanced Databases (BDDA)** course. Across six consecutive projects, a logistics management platform for a drone fleet is designed and progressively evolved, incorporating different data storage and processing paradigms.

The main objective is to understand when and why each technology should be used, building a polyglot persistence architecture where every component efficiently solves a specific problem.

## Evolutionary Architecture

| Project                                    | Main Technology     | Paradigm              | Objective                                              |
| ------------------------------------------ | ------------------- | --------------------- | ------------------------------------------------------ |
| `01-relational-databases-postgresql`       | PostgreSQL          | Relational            | Entity management and transactional persistence        |
| `02-distributed-key-value-redis`           | Redis               | Distributed Key-Value | Real-time telemetry, rankings, and geolocation         |
| `03-document-databases-mongodb`            | MongoDB             | Document Database     | Flexible storage of missions and events                |
| `04-search-engines-elasticsearch`          | Elasticsearch       | Search Engine         | Advanced search and forensic analysis                  |
| `05-wide-column-databases-cassandra`       | Apache Cassandra    | Wide-Column           | Large-scale time-series ingestion                      |
| `06-message-driven-architectures-rabbitmq` | RabbitMQ            | Asynchronous Messaging| Decoupling and distributed processing                  |

---

# 01 · Relational Databases with PostgreSQL

## Objective

Build the foundation of the system using a traditional relational architecture, implementing drone management and its core operations.

This project introduces fundamental concepts such as:

* Relational modeling
* SQL schema design
* CRUD APIs
* UUID identifiers
* Transactional persistence
* FastAPI and PostgreSQL integration

## Technologies

* PostgreSQL
* FastAPI
* SQLAlchemy
* Docker
* Grafana

## Outcome

A REST API is implemented with the ability to:

* Register drones
* Query fleet information
* Manage operational states
* Persist data consistently

---

# 02 · Distributed Key-Value Systems with Redis

## Objective

Address low-latency and high-write-frequency requirements by using Redis as a complementary system to PostgreSQL.

The following concepts are introduced:

* Hashes
* Sorted Sets
* GEO Indexes
* Distributed caching
* Polyglot persistence

## Technologies

* Redis
* PostgreSQL
* FastAPI
* Docker
* Grafana

## Outcome

The system incorporates:

* Real-time telemetry
* Efficiency rankings
* Geospatial queries
* Hybrid SQL + Redis management

---

# 03 · Document Databases with MongoDB

## Objective

Manage semi-structured information through a flexible document-oriented model.

Topics covered include:

* Schema-less design
* Asynchronous programming
* Document persistence
* Heterogeneous logs
* Mission history tracking

## Technologies

* MongoDB
* Motor (Async MongoDB Driver)
* FastAPI
* Pydantic

## Outcome

A complete mission management module is developed, capable of storing:

* Flight events
* Deliveries
* Inspections
* Alerts
* Specialized telemetry

without requiring rigid schemas.

---

# 04 · Search Engines with Elasticsearch

## Objective

Implement advanced search and analytics capabilities over millions of historical records.

Key concepts covered:

* Full-text search
* Query DSL
* Fuzzy Search
* Highlighting
* Aggregations
* Specialized indexing

## Technologies

* Elasticsearch
* Kibana
* FastAPI

## Outcome

An analytics system is built to:

* Detect incidents
* Search for complex patterns
* Correct typographical errors
* Generate real-time statistics

---

# 05 · Wide-Column Databases with Apache Cassandra

## Objective

Design a solution capable of handling massive write workloads for IoT telemetry.

Core concepts:

* Wide-Column Stores
* Partition Keys
* Clustering Columns
* Consistency Levels
* Time Series Data
* Query-Driven Modeling

## Technologies

* Apache Cassandra
* FastAPI
* Docker
* Grafana

## Outcome

The system evolves to manage:

* Millions of telemetry events
* Historical time-series records
* Optimized time-range queries
* Horizontal scalability

---

# 06 · Message-Driven Architectures with RabbitMQ

## Objective

Decouple producers and consumers through an event-driven architecture based on message queues.

Concepts covered:

* Event-Driven Architecture
* AMQP
* Publishers
* Consumers
* Exchanges
* Routing Keys
* Dead Letter Queues
* Asynchronous processing

## Technologies

* RabbitMQ
* aio-pika
* Cassandra
* Redis
* PostgreSQL
* FastAPI

## Outcome

The platform evolves into a distributed architecture where:

1. The API publishes events.
2. RabbitMQ distributes the messages.
3. Independent workers process the information.
4. Data is stored in different specialized engines.

This approach improves system performance, resilience, and scalability.

---

# Technologies Used

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