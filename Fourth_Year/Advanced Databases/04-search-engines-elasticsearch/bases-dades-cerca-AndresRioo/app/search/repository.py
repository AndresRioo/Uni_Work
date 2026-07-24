from elasticsearch import AsyncElasticsearch
from .models import IncidentReport, SearchQuery
from typing import List, Dict, Any

from datetime import datetime
from uuid import uuid4


class SearchRepository:
    def __init__(self, es_client: AsyncElasticsearch):
        self.es = es_client
        self.index_name = "incident_reports"

    async def create_index(self):
        """
        Challenge 1: Create the index with specific mappings.
        - 'description' and 'pilot_notes' should be analyzed as text.
        - 'severity' and 'component' should be keywords.
        - 'timestamp' should be a date.
        """

        # Mapping : indexar los campos 
        # mirar si ya existe
        exists = await self.es.indices.exists(index=self.index_name)
        if exists:
            return
        
        # Definir los indices
        index_config = {
            "settings": {
                "number_of_shards": 1,   
                "number_of_replicas": 1,
                "analysis": {
                    "analyzer": {
                        "default": {
                            "type": "standard"
                        }
                    }
                }
            },
            "mappings": {
                "properties": {
                    "id": {
                        "type": "keyword"
                    },
                    "dron_id": {
                        "type": "keyword"
                    },
                    "timestamp": {
                        "type": "date"
                    },
                    "severity": {
                        "type": "keyword"
                    },
                    "component": {
                        "type": "keyword"
                    },
                    "description": {
                        "type": "text"
                    },
                    "pilot_notes": {
                        "type": "text"
                    },
                    "environment_conditions": {
                        "type": "object",
                        "enabled": True
                    }
                }
            }
        }

        # Crear el indice
        try:
            await self.es.indices.create(
                index=self.index_name,
                body=index_config
            )
        except Exception  as e:
            raise RuntimeError(f"Error creating index: {str(e)}")

        pass

    async def index_incident_report(self, report: IncidentReport):
        """
        Challenge 2: Index a new forensic report.

        Como hacer un insert a la BDD
        """

        # asegurar que la tabla existe
        await self.create_index()
        
        # Asegurar el ID
        doc_id = report.id if report.id else str(uuid4())

        # Convertirlo de pydantic a diccionario
        document = report.dict()

        # Asegurar tipo de datos correctos
        # de UUID a string
        document["dron_id"] = str(document["dron_id"])

        # Convertir datetime a ISO format
        if isinstance(document["timestamp"], datetime):
            document["timestamp"] = document["timestamp"].isoformat()

        # Quitar los None 
        document = {k: v for k, v in document.items() if v is not None}

        # Indexar documento
        try:
            await self.es.index(
                index=self.index_name,
                id=doc_id,
                document=document,
                refresh=False  # lo hace buscable al instante, muy caro en producción 
            )
        except Exception  as e:
            raise RuntimeError(f"Error indexing document: {str(e)}")
        
        pass

    async def search_incidents(self, query: SearchQuery) -> List[IncidentReport]:
        """
        Challenge 3: Complex Search Logic
        - Full-text search in 'description' and 'pilot_notes'.
        - If 'query.fuzzy' is True, use fuzzy matching.
        - Filter by 'severity' or 'component' if provided.
        """

        # Crear el query

        # fuzzy matching : permite encontrar coincidencias aproximadas en lugar de exactas
        if query.fuzzy:
            text_query = {
                "multi_match": {
                    "query": query.text,
                    "fields": ["description", "pilot_notes"], # Buscar en varios campos a la vez
                    "fuzziness": "AUTO"
                }
            }
        else: # sin fuzzy 
            text_query = {
                "multi_match": {
                    "query": query.text,
                    "fields": ["description", "pilot_notes"]
                }
            }

        # Crear el filtro
        filters = []

        # si hay severity añadir como filter por ser keyword
        if query.severity:
            filters.append({
                "term": {
                    "severity": query.severity
                }
            })

        # si hay component añadir como filter por ser keyword
        if query.component:
            filters.append({
                "term": {
                    "component": query.component
                }
            })

        # Combinar en una query final
        # must : afecta el score, es más lento. Ayuda a buscar por relevancia 
        # filter : no afecta el score , más rápido. Ayuda a buscar valores exactos 
        es_query = {
            "bool": {
                "must": [text_query],
                "filter": filters
            }
        }

        # Hacer búsqueda
        try:
            response = await self.es.search(
                index=self.index_name,
                query=es_query
            )
        except Exception  as e:
            raise RuntimeError(f"Search error: {str(e)}")

        # Transformar los resultados
        results = []

        # iterar cada documento encontrado 
        for hit in response["hits"]["hits"]:

            # sacar el contenido del documento
            source = hit["_source"]

            # añadir manualmente el id
            source["id"] = hit["_id"]

            results.append(IncidentReport(**source))

        return results

    async def get_severity_stats(self) -> Dict[str, int]:
        """
        Challenge 4: Aggregations
        - Return count of incidents grouped by severity.
        """

        # SELECT severity, COUNT(*) FROM incidents GROUP BY severity;


        # Definir el query para hacer agregaciones 
        try:
            response = await self.es.search(
                index=self.index_name,
                size=0,  # no queremos documentos, solo stats
                aggs={
                    "by_severity": {
                        "terms": {  
                            "field": "severity"  # agrupar por terminos unicos de severity 
                        }
                    }
                }
            )
        except Exception  as e:
            raise RuntimeError(f"Aggregation error: {str(e)}")

        # Transformar los resultados
        buckets = response["aggregations"]["by_severity"]["buckets"]

        # guardar en un dict Dict[str, int]
        stats = {}

        # recorrer cada grupo 
        for bucket in buckets:

            # conseguir sus datos
            severity = bucket["key"]
            count = bucket["doc_count"]

            # actualizar dict 
            stats[severity] = count

        return stats