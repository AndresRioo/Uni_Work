import os

from pydantic import BaseSettings
from dotenv import load_dotenv
load_dotenv()

class Settings(BaseSettings):
    
    db_name: str = os.getenv("DB_NAME", "postgres")
    db_user: str = os.getenv("DB_USER", "postgres")
    db_password: str = os.getenv("DB_PASSWORD", "postgres")
    db_host: str = os.getenv("DB_HOST", "postgreSQL")
    db_port: str = os.getenv("DB_PORT", "5432")
    
    @property
    def db_url(self) -> str:
        return f"postgresql://{self.db_user}:{self.db_password}@{self.db_host}:{self.db_port}/{self.db_name}"
    
    redis_host: str = os.getenv("REDIS_HOST", "redis")
    redis_port: int = int(os.getenv("REDIS_PORT", 6379))
    redis_db: int = int(os.getenv("REDIS_DB", 0))

    mongo_url: str = os.getenv("MONGO_URL", "mongodb://mongodb:27017")
    mongo_db: str = os.getenv("MONGO_DB", "dron_logistics")
    
    class Config:
        env_file = ".env"
        env_file_encoding = "utf-8"