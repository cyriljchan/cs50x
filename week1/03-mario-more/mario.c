#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask for height
    int height;

    do
    {
        height = get_int("How tall is the pyramid? ");
    }
    while (height <= 0);

    for (int i = 1; i <= height; i++)
    {
        // Left side
        for (int j = i; j < height; j++)
        {
            printf(" ");
        }
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }

        // Middle spaces
        printf("  ");

        // Right side
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
