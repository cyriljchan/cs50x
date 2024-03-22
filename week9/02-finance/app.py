import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get session user details
    user = db.execute("SELECT * FROM users WHERE id==?", session["user_id"])[0]
    transactions = db.execute("""SELECT symbol, SUM(shares) as shares, price FROM transactions
                              WHERE id==?
                              GROUP BY symbol""", user["id"])

    # Calculate for current prices and the total sum, display index.html
    prices = []
    total = 0
    for transaction in transactions:
        prices.append(lookup(transaction["symbol"])["price"])
        total += lookup(transaction["symbol"])["price"] * transaction["shares"]
    return render_template("index.html", transactions=transactions, prices=prices, total=total, cash=user["cash"])


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged in!")
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


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Access /register through POST
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure username is not in use
        elif db.execute("SELECT * FROM users WHERE username==?", username):
            flash("Username already in use.")
            return redirect("/register")

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)

        # Ensure confirmation was submitted
        elif not confirmation:
            return apology("must provide password confirmation", 403)

        # Ensure password and confirmation matches
        elif password != confirmation:
            return apology("password does not match", 403)

        # Add user to users table, return to login
        else:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                       username, generate_password_hash(password))
            flash(f"User {username} has been registered!")
            return redirect("/login")

    # Render the register page
    else:
        return render_template("register.html")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Access /quote through POST
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))

        # Check if quoted symbol exists
        if quote:
            return render_template("quoted.html", quote=quote)
        else:
            return apology("invalid symbol", 400)

    # Render the quote page
    else:
        return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Access /buy through POST (send form)
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")
        cash = db.execute("SELECT cash FROM users WHERE id==?", session["user_id"])[0]["cash"]

        # Ensure symbol is valid
        if not quote:
            return apology("invalid symbol", 400)

        # Ensure shares value is a positive integer > 0
        elif not shares.isdigit() or int(shares) <= 0:
            return apology("invalid shares value", 400)

        # Ensure cash can handle transaction
        if float(shares) * quote["price"] > cash:
            return apology("not enough cash", 400)

        else:
            # Get session user info
            user = db.execute("SELECT * FROM users WHERE id==?", session["user_id"])[0]

            # Add BUY transaction into transactions table
            db.execute("INSERT INTO transactions (id, type, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, ?, ?)",
                       user["id"], "BUY", quote["symbol"], shares, quote["price"], datetime.now().strftime("%d/%m/%Y %H:%M:%S"))

            # Update session user's cash, return to login
            db.execute("UPDATE users SET cash=? WHERE id=?", (float(
                user["cash"]) - (float(shares) * quote["price"])), user["id"])
            flash(f"You have bought {shares} shares of {quote["symbol"]}!")
            return redirect("/")

    # Render the buy page
    else:
        return render_template("buy.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user = db.execute("SELECT * FROM users WHERE id==?", session["user_id"])[0]

    # Get a list of all stocks owned by session user
    symbols = db.execute("""SELECT symbol FROM transactions
                            WHERE id==?
                            GROUP BY symbol""", user["id"])

    # Access /sell through POST
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        total_shares = db.execute("""SELECT SUM(shares) FROM transactions
                                    WHERE symbol==?
                                    GROUP BY symbol""", symbol)

        # Ensure a symbol is recieved
        if not symbol:
            return apology("no symbol", 400)

        # Ensure shares value is a positive integer > 0
        elif not shares or int(shares) <= 0 or int(shares) > total_shares[0]["SUM(shares)"]:
            return apology("invalid shares value", 400)

        else:

            # Get session user's info and current market price of the stock
            user = db.execute("SELECT * FROM users WHERE id==?", session["user_id"])[0]
            price = lookup(symbol)["price"]

            # Insert "SELL" transaction into transactions table
            db.execute("INSERT INTO transactions (id, type, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, ?, ?)",
                       user["id"], "SELL", symbol, -int(shares), price, datetime.now().strftime("%d/%m/%Y %H:%M:%S"))

            # Update session user's cash, return to index
            db.execute("UPDATE users SET cash=? WHERE id=?",
                       user["cash"] + (float(shares) * price), user["id"])
            flash(f"You sold {shares} shares of {symbol}.")
            return redirect("/")

    else:
        return render_template("sell.html", symbols=symbols)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get all transaction details
    transactions = db.execute("""SELECT * FROM transactions
                              ORDER BY transacted DESC""")

    # Calculate the sum total of all transactions
    total = 0
    print("\n\n\n\n\n\n\n\n\n\n", transactions)
    for transaction in transactions:
        type = 1 if transaction["type"] == "BUY" else -1
        total += type * transaction["shares"] * (transaction["price"] * type)

    # Render the transactions page
    return render_template("history.html", transactions=transactions, total=total)
