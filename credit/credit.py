from cs50 import get_string

lenght = 0

def main():
    word = get_string("Number: ")
    lenght = len(word)
    two_dig = int(word[0] + word[1])

    valid = check(word)

    if not valid:
        print("INVALID")
    elif (lenght == 13 or lenght == 16) and word[0] == '4':
        print("VISA")
    elif lenght == 15 and (two_dig == 34 or two_dig == 37):
        print("AMEX")
    elif lenght == 16 and two_dig > 50 and two_dig < 56:
        print("MASTERCARD")
    else:
        print("INVALID")

def check(num):
    digits = []
    for i in range(lenght):
        dig = int(num[i])
        if i % 2 == 0:
            if dig < 5:
                digits.append(dig * 2)
            else:
                digits.append(1)
                digits.append((dig * 2) % 10)
        else:
            digits.append(dig)
    total = sum(digits)
    return total % 10 == 0

main()
