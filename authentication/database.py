from pymongo import MongoClient
from app import app

# secret key for user session
app.secret_key = "ITSASECRET"

mongoconnection = MongoClient("mongodb+srv://gmsDavid:gmsDavid$@cluster0.4xth9.mongodb.net/myFirstDatabase?retryWrites=true&w=majority")


db = mongoconnection["gms"]["gms1"]
# coll = db["gms1"]

# db2 = mongoconnection["test"]
# coll2 = db2["test1"]


# post1 = {"name":"david", "age":10}
# post2 = {"name":"joanes", "age":13}

# coll2.insert_many([post1, post2])

# password = getHashed("tonye")
# result = db.find_one({"username":"joanesk", "password":password})
# for x in result:
#     print(x)
