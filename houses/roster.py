from cs50 import SQL
from sys import argv

db = SQL("sqlite:///students.db")

def main():
    if len(argv) != 2:
        print("Missing comand-line argument")
        return
    lista = db.execute("SELECT first,middle,last,birth FROM students WHERE house=? ORDER BY last;", argv[1])
    for row in lista:
        if row['middle'] == None:
            print(f"{row['first']} {row['last']}, born {row['birth']}")
        else:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")


main()
