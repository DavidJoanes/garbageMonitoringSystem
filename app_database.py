#========================Backend================#
import sqlite3
from passlib.hash import sha256_crypt as shc

#------------------------------Create main-table (for accounts)----------------------------#
def createMainTable():
    con = sqlite3.connect("mainDatabase.db")
    cur = con.cursor()
    cur.execute("CREATE TABLE IF NOT EXISTS AccountRegister (id INTEGER PRIMARY KEY, Firstname text, Lastname text,\
                Username text, Password text, ConfirmPassword text)")
    con.commit()
    con.close()

def addAccount(Firstname, Lastname, Username, Password, ConfirmPassword):
    con = sqlite3.connect("mainDatabase.db")
    cur = con.cursor()
    hashPassword = shc.hash(Password)
    hashConfirmPassword = shc.hash(ConfirmPassword)
    if Password == ConfirmPassword:
        cur.execute("INSERT INTO AccountRegister VALUES (?,?,?,?,?,?)",
                    (Firstname, Lastname, Username, hashPassword, hashConfirmPassword))
    else:
        return False
    # con.commit()
    # con.close()

def checkAccount(Username, Password):
    con = sqlite3.connect("mainDatabase.db")
    cur = con.cursor()
    cur.execute("SELECT Username, Password FROM AccountRegister")
    rows = cur.fetchall()
    for (user, pw) in rows:
        check = shc.verify(Password, pw)
        if Username==user and check:
            return True
    return False
    # con.close()
    # return rows
