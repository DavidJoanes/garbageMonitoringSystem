from flask import Flask
app = Flask(__name__)

from views import *

if __name__ == "__main__":
    app.run('0.0.0.0', 5000, debug=True, threaded=True)