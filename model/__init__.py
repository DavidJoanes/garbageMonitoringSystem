from app import app
from flask import request, session
from authentication.database import *
from authentication.hashpassword import *
from bson import json_util, ObjectId
import json


def checkloginusername():
    username = request.form["username"]
    check = db.find_one({"username": username})
    if check is None:
        return "No User"
    # elif check == username:
    #     return f"User - {username}"
    else:
        return f"User - {username}"


def checkloginpassword():
    username = request.form["username"]
    check = db.find_one({"username": username})
    password = request.form["password"]
    hashpassword = getHashed(password)
    if hashpassword == check["password"]:
        session["username"] = username
        return "correct"
    else:
        return "wrong"
    

def checkusername():
    username = request.form["username"]
    check = db.find_one({"username": username})
    if check is None:
        return "Available"
    else:
        return "Username taken"


def registerUser():
    fields = [k for k in request.form]                                      
    values = [request.form[k] for k in request.form]
    data = dict(zip(fields, values))
    user_data = json.loads(json_util.dumps(data))
    user_data["password"] = getHashed(user_data["password"])
    user_data["confirmpassword"] = getHashed(user_data["confirmpassword"])
    db.insert(user_data)
    print("Done")