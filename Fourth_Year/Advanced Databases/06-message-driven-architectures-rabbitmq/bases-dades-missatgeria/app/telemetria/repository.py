from cassandra.query import SimpleStatement, PreparedStatement
from cassandra import ConsistencyLevel
from datetime import datetime
from typing import List, Optional
from app.cassandra_client import cassandra_client
from app.telemetria.models import Telemetria, TelemetriaCreate

from datetime import datetime, timezone
from cassandra import ReadTimeout, WriteTimeout


class TelemetriaRepository:

    def __init__(self):


        if cassandra_client.get_session() is None:
            cassandra_client.connect_sync()

        self.session = cassandra_client.get_session()

        if self.session is None:
            raise Exception("Cassandra no disponible")

        self._prepare_table()
        # TODO: Preparar el statement d'inserció per a optimitzar les escriptures (Prepared Statements)
        # self.insert_stmt = None

        # hacerlo prepared porque se ejecuta muchas veces y es fija
        self.insert_stmt = self.session.prepare("""
            INSERT INTO telemetria (
                dron_id,
                timestamp,
                latitud,
                longitud,
                altitud,
                bateria,
                velocidad_viento,
                temperatura_motor
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        """)

        # usar tambien un prepared statement en stats
        self.stats_stmt = self.session.prepare("""
            SELECT 
                AVG(bateria) AS avg_bateria,
                MAX(altitud) AS max_altitud,
                AVG(velocidad_viento) AS avg_viento
            FROM telemetria
            WHERE dron_id = ?
        """)

    def _prepare_table(self):
        """

        dron_id - Partition Key
        timestamp - Clustering Column

        Esto garantiza:

        Todos los datos de un dron están juntos
        Ordenados por tiempo

        TODO: Crear la taula 'telemetria' si no existeix.
        Penseu bé en la Partition Key i la Clustering Column per a optimitzar les consultes
        per dron_id i timestamp (ordenat descendentment).
        """

        query = """
        CREATE TABLE IF NOT EXISTS telemetria (
            dron_id INT,
            timestamp TIMESTAMP,
            latitud DOUBLE,
            longitud DOUBLE,
            altitud DOUBLE,
            bateria DOUBLE,
            velocidad_viento DOUBLE,
            temperatura_motor DOUBLE,
            PRIMARY KEY ((dron_id), timestamp)
        ) WITH CLUSTERING ORDER BY (timestamp DESC);
        """
        self.session.execute(query)
        

    def save(self, telemetria: TelemetriaCreate, consistency_level: ConsistencyLevel = ConsistencyLevel.ONE) -> Telemetria:
        """
        TODO: Implementar la lògica de guardat.
        1. Generar el timestamp actual.
        2. Executar la inserció utilitzant el prepared statement.
        3. Configurar el nivell de consistència (Tunable Consistency).
        """
        # Timestamp actual UTC Coordinated Universal Time
        now = datetime.now(timezone.utc)


        dron_id = telemetria.dron_id

        # Bind de valores al prepared statement
        bound = self.insert_stmt.bind((
            dron_id,
            now,
            telemetria.latitud,
            telemetria.longitud,
            telemetria.altitud,
            telemetria.bateria,
            telemetria.velocidad_viento,
            telemetria.temperatura_motor
        ))

        # Tunable Consistency
        bound.consistency_level = consistency_level

        # Ejecutar query
        self.session.execute(bound)

        # Retornar objeto
        return Telemetria(**telemetria.dict(), timestamp=now)

    def get_by_dron(self, dron_id: int, start_time: Optional[datetime] = None, end_time: Optional[datetime] = None) -> List[Telemetria]:
        """
        TODO: Recuperar la telemetria d'un dron específic.
        S'ha de permetre opcionalment filtrar per un rang de temps (start_time, end_time).
        Aprofiteu que les dades estan ordenades per Clustering Column.
        """


        # no hace falta hacer "ORDER BY timestamp DESC" porque ya esta ordenado 
        # tampoco hacerla prepared al ser dinamica 
        query = """
            SELECT dron_id, timestamp, latitud, longitud, altitud,
                bateria, velocidad_viento, temperatura_motor
            FROM telemetria
            WHERE dron_id = %s
        """

        params = [dron_id]

        # Filtros por rango de tiempo (clustering column)
        if start_time and end_time:
            query += " AND timestamp >= %s AND timestamp <= %s"
            params.extend([start_time, end_time])

        elif start_time:
            query += " AND timestamp >= %s"
            params.append(start_time)

        elif end_time:
            query += " AND timestamp <= %s"
            params.append(end_time)

        query += " LIMIT 1000"
        
        # Ejecutar query
        try:
            rows = self.session.execute(query, tuple(params))
        except (ReadTimeout, WriteTimeout) as e:
            raise Exception(f"Cassandra timeout: {str(e)}")
        except Exception as e:
            raise Exception(f"Cassandra error: {str(e)}")

        # Mapear resultados a modelo
        return [
            Telemetria(
                dron_id=row.dron_id,
                timestamp=row.timestamp,
                latitud=row.latitud,
                longitud=row.longitud,
                altitud=row.altitud,
                bateria=row.bateria,
                velocidad_viento=row.velocidad_viento,
                temperatura_motor=row.temperatura_motor
            )
            for row in rows
        ]

    def get_stats(self, dron_id: int) -> dict:
        """
        TODO: Calcular estadístiques agregades per un dron utilitzant funcions nàtives de Cassandra (AVG, MAX).
        Retornar un diccionari amb avg_bateria, max_altitud i avg_viento.
        """

        try:
            row = self.session.execute(self.stats_stmt, (dron_id,)).one()
        except (ReadTimeout, WriteTimeout) as e:
            raise Exception(f"Cassandra timeout: {str(e)}")
        except Exception as e:
            raise Exception(f"Cassandra error: {str(e)}")

        # Si no hay datos 
        if not row or row.avg_bateria is None:
            return {}

        # evitar el caso donde devuleve None en lugar de un Float 
        return {
            "avg_bateria": float(row.avg_bateria) if row.avg_bateria is not None else 0.0,
            "max_altitud": float(row.max_altitud) if row.max_altitud is not None else 0.0,
            "avg_viento": float(row.avg_viento) if row.avg_viento is not None else 0.0
        }