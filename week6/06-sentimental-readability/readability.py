from cs50 import get_string
from string import ascii_letters


def main():
    readability()


def count(text: str, key: str) -> int:
    if len(text) == 0:
        return 0

    return len([c for c in text if c in key])


def coleman(text: str) -> int:
    letters = count(text, ascii_letters)
    words = count(text, " ") + 1
    sentences = count(text, ".?!")

    L = letters * 100.00 / words
    S = sentences * 100.00 / words
    index = round(0.0588 * L - 0.296 * S - 15.8)

    return index


def readability():
    text = get_string("Text: ")
    index = coleman(text)
    if index < 0:
        grade = "Before Grade 1"
    elif index > 16:
        grade = "Grade 16+"
    else:
        grade = f"Grade {index}"
    print(grade)


main()
