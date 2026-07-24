from motor.motor_asyncio import AsyncIOMotorClient
import asyncio
from .settings import Settings

settings = Settings()

class MongoDBClient:
    client: AsyncIOMotorClient = None
    db = None

    @classmethod
    def get_db(cls):
        # Check if we have a client AND if the loop it's tied to is still running
        if cls.client is None:
            cls.client = AsyncIOMotorClient(settings.mongo_url)
            cls.db = cls.client[settings.mongo_db]
        else:
            try:
                # Try to get the current loop and see if it's the same
                loop = asyncio.get_running_loop()
                if cls.client.get_io_loop() != loop:
                    cls.client = AsyncIOMotorClient(settings.mongo_url)
                    cls.db = cls.client[settings.mongo_db]
            except RuntimeError:
                # No running loop, just return what we have (might fail later)
                pass
        return cls.db

    @classmethod
    def close_connection(cls):
        if cls.client is not None:
            cls.client.close()
            cls.client = None

def get_mongodb():
    return MongoDBClient.get_db()
