from cs50 import get_float, get_int, get_string


def main():
    mario()


def mario():
    levels = get_int("Height: ")
    while levels <= 0 or levels > 8:
        levels = get_int("Height: ")
    for i in range(1, levels + 1):
        print(f"{' ' * (levels - i)}{'#' * (i)}  {'#' * (i)}")


main()
