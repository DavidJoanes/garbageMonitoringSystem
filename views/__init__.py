from flask import render_template, request, redirect, url_for, session
from app import app
from model import *

@app.route('/', methods=["GET"])
# @app.route('/')
def home():
    # return render_template("index.html")
    if "username" in session:
        return render_template('index.html')
    else:
        return render_template('login.html')

# Register new user
@app.route('/register', methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        registerUser()
        return redirect(url_for("login"))

#Check if username already exists in the registratiion page
@app.route('/checkusername', methods=["POST"])
def check():
    # pass
    return checkusername()

# Everything Login (Routes to renderpage, check if username exist and also verifypassword through Jquery AJAX request)
@app.route('/login', methods=["GET"])
def login():
    if request.method == "GET":
        if "username" not in session:
            return render_template("login.html")
        else:
            return redirect(url_for("home"))


@app.route('/checkloginusername', methods=["POST"])
def checkUserlogin():
    # pass
    return checkloginusername()

@app.route('/checkusernamelogin', methods=["GET"])
def checkloginUser():
    if request.method == "GET":
        if "username" in session:
            return checkloginusername()

@app.route('/checkloginpassword', methods=["POST"])
def checkUserpassword():
    # pass
    return checkloginpassword()

#The admin logout
@app.route('/logout', methods=["GET"])  # URL for logout
def logout():  # logout function
    session.pop('username', None)  # remove user session
    return redirect(url_for("home"))  # redirect to home page with message

#Forgot Password
@app.route('/forgot-password', methods=["GET"])
def forgotpassword():
    return render_template('forgot-password.html')

#404 Page
@app.route('/404', methods=["GET"])
def errorpage():
    return render_template("404.html")

#Blank Page
@app.route('/blank', methods=["GET"])
def blank():
    return render_template('blank.html')