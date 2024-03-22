from cs50 import get_float, get_int, get_string
from math import pow


def main():
    credit()


def digitsToList(number: int) -> list:
    return [int(digit) for digit in str(number)]


def luhn(number: int) -> bool:
    numberlist = digitsToList(number)
    odd = sum([int(num) for num in numberlist[-1::-2]])
    even = sum([sum(digitsToList(int(num) * 2)) for num in numberlist[-2::-2]])
    return (odd + even) % 10


def credit():
    number = get_int("Number: ")
    numberlen = len(str(number))
    if not (luhn(number)):
        if str(number)[0] in ["4"] and (numberlen == 13 or numberlen == 16):
            print("VISA")
        elif str(number)[0:2] in ["34", "37"] and numberlen == 15:
            print("AMEX")
        elif str(number)[0:2] in ["51", "52", "53", "54", "55"] and (
            numberlen == 15 or numberlen == 16
        ):
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


main()
