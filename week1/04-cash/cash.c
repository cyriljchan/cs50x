#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int change;
    int coins = 0;
    do
    {
        change = get_int("Change owed: ");
    }
    while (change < 0);

    // 25c
    if (change >= 25)
    {
        coins += change / 25;
        change = change % 25;
    }

    // 10c
    if (change >= 10)
    {
        coins += change / 10;
        change = change % 10;
    }

    // 5c
    if (change >= 5)
    {
        coins += change / 5;
        change = change % 5;
    }

    // 1c
    if (change >= 1)
    {
        coins += change / 1;
    }

    // Print coins used
    printf("%d\n", coins);
}
