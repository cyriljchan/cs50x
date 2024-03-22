from cs50 import get_float, get_int, get_string


def main():
    hello()


def hello():
    name = get_string("What is your name? ")
    print(f"hello, {name}")


main()
