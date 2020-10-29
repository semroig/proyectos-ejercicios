import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    total = []

    tabla = db.execute("SELECT SUM(shares),symbol FROM history WHERE user_id=:user GROUP BY symbol HAVING SUM(shares)<>0;",
    user=session['user_id'])

    for i in tabla:
        info = lookup(i['symbol'])
        i['price'] = usd(info['price'])
        i['name'] = info['name']
        i['total'] = usd(i['SUM(shares)'] * info['price'])
        total.append(i['SUM(shares)'] * info['price'])

    cash = db.execute("SELECT cash FROM users WHERE id=:user;", user=session['user_id'])
    total.append(cash[0]['cash'])

    return render_template("index.html", tabla=tabla, cash=usd(cash[0]['cash']), total=usd(sum(total)))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide a symbol", 403)

        elif not request.form.get("shares"):
            return apology("must provide number of shares", 403)

        resp = lookup(request.form.get("symbol"))

        if not resp:
            return apology("must provide a valid symbol", 403)
        else:
            afford = resp['price'] * float(request.form.get("shares"))
            money = db.execute("SELECT cash FROM users WHERE id=:user", user=session["user_id"])
            if money[0]['cash'] < afford:
                return apology("can't afford", 403)
            else:
                db.execute("INSERT INTO history VALUES (:user,:shares,:symbol,:price,(SELECT datetime('now')))",
                user=session["user_id"], shares=int(request.form.get("shares")), symbol=resp['symbol'],
                price=resp["price"])

                spent = money[0]['cash'] - afford

                db.execute("UPDATE users SET cash=:result WHERE id=:user",
                result=spent, user=session["user_id"])

                return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT * FROM history WHERE user_id=:user;", user=session['user_id'])

    return render_template("history.html", tabla=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide a symbol", 403)

        resp = lookup(request.form.get("symbol"))
        if not resp:
            return apology("must provide a valid symbol", 403)
        else:
            return render_template("quoted.html",
            name=resp['name'], symbol=resp['symbol'], price=usd(resp['price']))

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif not request.form.get("confirmation"):
            return apology("must provide a confirmation password", 403)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("must confirm the same password", 403)

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
        username=request.form.get("username"),
        password=generate_password_hash(request.form.get("password")))

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    own = db.execute("SELECT SUM(shares),symbol FROM history WHERE user_id=:user GROUP BY symbol HAVING SUM(shares)<>0;",
    user=session['user_id'])

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide a symbol", 403)

        elif not request.form.get("shares"):
            return apology("must provide number of shares", 403)

        resp = lookup(request.form.get("symbol"))

        if not resp:
            return apology("must provide a valid symbol", 403)

        for i in own:

            if resp['symbol'] == i['symbol']:

                if i['SUM(shares)'] < int(request.form.get("shares")):
                    return apology("not enough shares to sell", 403)

                else:
                    earn = resp['price'] * float(request.form.get("shares"))
                    money = db.execute("SELECT cash FROM users WHERE id=:user", user=session["user_id"])
                    db.execute("INSERT INTO history VALUES (:user,:shares,:symbol,:price,(SELECT datetime('now')))",
                    user=session["user_id"], shares=-int(request.form.get("shares")), symbol=resp['symbol'],
                    price=resp["price"])

                    neto = money[0]['cash'] + earn

                    db.execute("UPDATE users SET cash=:result WHERE id=:user",
                    result=neto, user=session["user_id"])

                    return redirect("/")

    else:
        return render_template("sell.html", own=own)


@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    if request.method == "POST":

        if not request.form.get("password"):
            return apology("must provide your old password", 403)

        elif not request.form.get("new"):
            return apology("must provide new password", 403)

        elif not request.form.get("confirmation"):
            return apology("must provide new password twice", 403)

        elif request.form.get("new") != request.form.get("confirmation"):
            return apology("must confirm the same password", 403)

        dataso = db.execute("SELECT * FROM users WHERE id = :user", user=session['user_id'])

        # Ensure username exists and password is correct
        if len(dataso) != 1 or not check_password_hash(dataso[0]["hash"], request.form.get("password")):
            return apology("invalid old password", 403)

        else:
            db.execute("UPDATE users SET hash=:new WHERE id=:user",
            new=generate_password_hash(request.form.get("new")), user=session["user_id"])

        return redirect("/")

    else:
        return render_template("settings.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
