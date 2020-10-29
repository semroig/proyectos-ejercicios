import csv
from cs50 import SQL
from sys import argv

db = SQL("sqlite:///students.db")

def main():
    if len(argv) != 2:
        print("Missing comand-line argument")
        return
    with open(argv[1], "r") as students:
        reader = csv.DictReader(students)
        for row in reader:
            names = row["name"].split()
            if len(names) < 3:
                db.execute("INSERT INTO students (first, last, house, birth) \
                VALUES (?, ?, ?, ?);", names[0], names[1], row['house'], row['birth'])
            else:
                db.execute("INSERT INTO students (first, middle, last, house, birth) \
                VALUES (?, ?, ?, ?, ?);", names[0], names[1], names[2], row['house'], row['birth'])


main()