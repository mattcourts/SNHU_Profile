
from pymongo import MongoClient
from bson.objectid import ObjectId


class MongoDBConnector:

    def __init__(self):
        # globals
        self.HOST = ''
        self.PORT = -1
        self.DB = ''
        self.COL = ''

    def __validate_properties(self):
        if (self.HOST == '' or self.PORT == '' or self.DB == '' or self.COL == ''):
            print(' DB configuration value missing: \n MongoDBConnector.set-user\(username\) \n MongoDBConnector.set-pass\(passkey\) \n self.HOST = hostname \n self.PORT = port number \n self.DB = db name \n self.COL = collection name')
            return False
        else:
            return True

    def connect(self, USER, PASS):
        if (not self.__validate_properties()):
            return False
        self.client = MongoClient('mongodb://%s:%s@%s:%d' %
                                  (USER, PASS, self.HOST, self.PORT))
        self.database = self.client['%s' % (self.DB)]
        self.collection = self.database['%s' % (self.COL)]
        return True

    def close(self):
        self.client.close()


class AnimalShelter(MongoDBConnector):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self):
        super()
        self.HOST = 'nv-desktop-services.apporto.com'
        self.PORT = 30081
        self.DB = 'aac'
        self.COL = 'animals'


# Complete this create method to implement the C in CRUD.


    def create(self, data):
        if data is not None:
            self.database.animals.insert_one(data)  # data should be dictionary
            return True
        else:
            raise Exception("Nothing to save, because data parameter is empty")
            return False

# Create method to implement the R in CRUD.
    def read(self, searchData):
        if searchData is not None:
            data = list(self.database.animals.find(searchData, {"_id": False}))
            return data
        else:
            Exception("Nothing to save, because data parameter is empty")
            return None


# Create method to implement the U in CRUD.


    def update(self, searchData, updateData):
        if searchData is not None:
            result = self.database.animals.update_many(
                searchData, {"$set": updateData})
        else:
            return "{}"
        return result.raw_result
# Create method to implement the D in CRUD.

    def delete(self, deleteData):
        if deleteData is not None:
            result = self.database.animals.delete_many(deleteData)
        else:
            return "{}"
        return result.raw_result

