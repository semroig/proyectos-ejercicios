from sys import argv
import csv

def main():
    if len(argv) != 3:
        print("Missing comand-line argument")
        return

    dna = open(argv[2])
    seq = dna.read()
    file = open(argv[1])
    reader = csv.reader(file)

    for row in reader:
        ternas = row[1:]
        break

    person = []

    for STR in ternas:
        record = []
        for i in range(len(seq)):
            cons = -1
            sub = STR
            while sub == STR:
                sub = seq[i:i + len(STR)]
                i = i + len(STR)
                cons += 1
            record.append(cons)
        person.append(str(max(record)))

    for row in reader:
        base = row[1:]
        if base == person:
            print(row[0])
            dna.close()
            file.close()
            return

    print("No match")
    dna.close()
    file.close()


main()