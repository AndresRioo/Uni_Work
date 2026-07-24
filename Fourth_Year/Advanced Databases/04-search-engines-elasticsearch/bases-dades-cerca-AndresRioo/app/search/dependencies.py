import os
from elasticsearch import AsyncElasticsearch

"""
class ElasticsearchClient:
    _instance = None

    @classmethod
    def get_client(cls):
        if cls._instance is None:
            es_url = os.getenv("ELASTICSEARCH_URL", "http://elasticsearch:9200")
            cls._instance = AsyncElasticsearch(
                es_url,
                verify_certs=False,
                request_timeout=30
            )
        return cls._instance

    @classmethod
    async def close(cls):
        if cls._instance:
            await cls._instance.close()
            cls._instance = None

def get_elasticsearch():
    return ElasticsearchClient.get_client()
"""

from elasticsearch import AsyncElasticsearch
import os

async def get_elasticsearch():
    es_url = os.getenv("ELASTICSEARCH_URL", "http://elasticsearch:9200")
    client = AsyncElasticsearch(
        es_url,
        verify_certs=False,
        request_timeout=30
    )
    try:
        yield client
    finally:
        await client.close()