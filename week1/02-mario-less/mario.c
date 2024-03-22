#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask for height
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0);
    for (int i = 0; i < height; i++)
    {
        // Print spaces
        for (int j = 1; j < height - i; j++)
        {
            printf(" ");
        }

        // Print #
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
