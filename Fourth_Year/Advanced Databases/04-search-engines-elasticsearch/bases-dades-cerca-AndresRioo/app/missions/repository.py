from motor.motor_asyncio import AsyncIOMotorDatabase
from typing import List, Optional
from .models import Mission, MissionCreate, MissionUpdate, MissionEvent
from bson import ObjectId
from uuid import UUID
from bson.errors import InvalidId

class MissionRepository:
    def __init__(self, db: AsyncIOMotorDatabase):
        self.collection = db["missions"]

    async def create_mission(self, mission: MissionCreate) -> str:
        """
        TODO: Implement mission creation in MongoDB.
        Requirement: Missions are semi-structured; ensure the initial document 
        is created with an empty list of logs.
        """

        # creamos un documento con la estructura de MissionCreate
        document = {
            "dron_id": str(mission.dron_id),
            "payload_type": mission.payload_type,
            "mission_objective": mission.mission_objective,
            "start_time": mission.start_time,
            "status": "scheduled",
            "logs": []
        }

        result = await self.collection.insert_one(document)

        # Mongo no serializa UUID automáticamente, así que hay que convertirlo:
        return str(result.inserted_id)


    async def get_mission(self, mission_id: str) -> Optional[dict]:
        """
        TODO: Retrieve a mission by its MongoDB ObjectId.
        """

        # para retrieve _id hace falta usar ObjectID
        try:
            id_Mongo = ObjectId(mission_id)
        except InvalidId: # en caso de id invalido 
            return None

        mission = await self.collection.find_one({"_id": id_Mongo})

        if mission is None:
            return None

        # para poder serializar
        mission["_id"] = str(mission["_id"])

        # devolver string
        return mission

    async def get_missions_by_dron(self, dron_id: UUID) -> List[dict]:
        """
        TODO: Retrieve all missions associated with a specific Drone UUID.
        """

        # buscar documento y convertir en una lista
        cursor = self.collection.find({"dron_id": str(dron_id)}) # dron id esta en string
        missions = await cursor.to_list(length=1000) # usar length 1000 para evitar tener todos los docs en memoria

        for mission in missions:
            mission["_id"] = str(mission["_id"])

        return missions

    async def add_event_to_mission(self, mission_id: str, event: MissionEvent) -> bool:
        """
        TODO: Use MongoDB's $push operator to add an event to the mission's logs.
        Requirement: This must be an atomic operation.
        """

        # para retrieve _id hace falta usar ObjectID
        try:
            id_Mongo = ObjectId(mission_id)
        except InvalidId: # en caso de id invalido 
            return None

        # transformar el ID para buscar el doc
        # ejecutar push para añadir los logs 
        result = await self.collection.update_one(
            {"_id": id_Mongo},
            {
                "$push": {
                    "logs": event.dict()
                }
            }
        )

        # si devuelve 0 la mision no existe
        return result.matched_count > 0

    async def update_mission_status(self, mission_id: str, update: MissionUpdate) -> bool:
        """
        TODO: Update the status and/or end_time of a mission.
        """

        # añadir los campos a modificar
        update_fields = {}

        if update.status is not None:
            update_fields["status"] = update.status

        if update.end_time is not None:
            update_fields["end_time"] = update.end_time

        # si no hay campos, no ejecutamos nada
        if not update_fields:
            return False
        
        # para retrieve _id hace falta usar ObjectID
        try:
            id_Mongo = ObjectId(mission_id)
        except InvalidId: # en caso de id invalido 
            return None

        

        result = await self.collection.update_one(
            {"_id": id_Mongo},
            {"$set": update_fields}
        )

        return result.matched_count > 0

    async def search_missions_by_objective(self, keyword: str) -> List[dict]:
        """
        TODO: Implement a text-based search or regex search in the mission_objective.
        """

        # con regex buscar con la keyword
        # con options "i" ignoramos mayuscula 
        cursor = self.collection.find(
            {
                "mission_objective": {
                    "$regex": keyword,
                    "$options": "i"
                }
            }
        )

        # Convertir cursor a lista
        missions = await cursor.to_list(length=1000)

        # transformar al formato correcto
        for mission in missions:
            mission["_id"] = str(mission["_id"])

        return missions