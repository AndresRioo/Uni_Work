from cassandra.cluster import Cluster
from cassandra.auth import PlainTextAuthProvider
from app.settings import Settings
import time
from cassandra.cluster import NoHostAvailable

settings = Settings()

class CassandraClient:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(CassandraClient, cls).__new__(cls)
            cls._instance.cluster = None
            cls._instance.session = None
        return cls._instance

    async def connect(self):
        for i in range(10):
            try:
                print(f"Cassandra intento {i+1}/10...")

                # repetir la conexión hasta que se conecte
                self.cluster = Cluster([settings.cassandra_hosts])
                self.session = self.cluster.connect()

                self._prepare_keyspace()
                print("Cassandra conectada")
                return

            except NoHostAvailable as e:
                print(f"Cassandra no lista, intento {i+1}/10...")
                time.sleep(5)

        raise Exception("No se pudo conectar a Cassandra")

    def _prepare_keyspace(self):
        self.session.execute(
            f"""
            CREATE KEYSPACE IF NOT EXISTS {settings.cassandra_keyspace}
            WITH replication = {{'class': 'SimpleStrategy', 'replication_factor': '1'}}
            """
        )
        self.session.set_keyspace(settings.cassandra_keyspace)

    def get_session(self):
        return self.session

    def close(self):
        self.cluster.shutdown()

    def connect_sync(self, retries=30, delay=2):
        for i in range(retries):
            try:
                print(f"Cassandra intento {i+1}/{retries}...")
                
                self.cluster = Cluster([settings.cassandra_hosts])
                self.session = self.cluster.connect()
                self.session.execute("""
                    CREATE KEYSPACE IF NOT EXISTS telemetria_keyspace
                    WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1}
                """)
                self.session.set_keyspace("telemetria_keyspace")
                                
                print("Cassandra conectada ")
                return
            
            except Exception as e:
                print(f"Cassandra no lista: {e}")
                
                # resetear cluster
                if self.cluster:
                    try:
                        self.cluster.shutdown()
                    except:
                        pass
                
                self.cluster = None
                self.session = None
                
                time.sleep(delay)

        raise Exception("No se pudo conectar a Cassandra")

cassandra_client = CassandraClient()
