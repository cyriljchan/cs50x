from cs50 import get_float


def main():
    cash()


def cash():
    while (True):
        print(9)
        change = get_float("Change: ")
        if change > 0:
            break
    coins = 0

    # 25c
    if change >= 0.25:
        coins += change // 0.25
        change = round(change % 0.25, 2)
        # print(25, coins, change)

    # 10c
    if change >= 0.10:
        coins += change // 0.10
        change = round(change % 0.10, 2)
        # print(10, coins, change)

    # 5c
    if change >= 0.05:
        coins += change // 0.05
        change = round(change % 0.05, 2)
        # print(5, coins, change)

    # 1c
    if change >= 0.01:
        coins += change // 0.01
        change = round(change % 0.01, 2)
        # print(1, coins, change)

    print(f"{int(coins)}")


main()
