#include <cs50.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

// Prototype
int longlen(long n);
bool startMatch(long card_num, int start);
int *longToArray(long n);
void reverseArray(int arr[], int start, int end);
int luhn(long card);

int main(void)
{
    long card = get_long("Number: ");

    // Check card validity
    if (longlen(card) == 15 && (startMatch(card, 34) || startMatch(card, 37)) && luhn(card))
    {
        printf("AMEX\n");
    }
    else if ((longlen(card) == 16) &&
             (startMatch(card, 51) || startMatch(card, 52) || startMatch(card, 53) || startMatch(card, 54) ||
              startMatch(card, 55)) &&
             luhn(card))
    {
        printf("MASTERCARD\n");
    }
    else if ((longlen(card) == 13 || longlen(card) == 16) && startMatch(card, 4) && luhn(card))
    {
        printf("VISA\n");
    }

    else
    {
        printf("INVALID\n");
    }
}

// Length of long
int longlen(long n)
{
    int length = 0;
    while (n > 0)
    {
        n = n / 10;
        length++;
    }

    return length;
}

// Match starting number(s)
bool startMatch(long card, int start)
{
    while (longlen(card) > longlen(start))
    {
        card = card / 10;
    }

    return (card == start);
}

// Creates an array from long in reverse
int *longToArray(long number)
{
    int *arr = malloc(sizeof(long) * longlen(number));
    int i = 0;
    while (number != 0)
    {
        arr[i] = number % 10;
        number /= 10;
        i++;
    }

    return arr;
}

// Gets the sum of the digits of a number
int sumOfDigits(int number)
{
    int sum = 0;
    while (number != 0)
    {
        sum += number % 10;
        number /= 10;
    }

    return sum;
}

// Luhn's Algorithm
int luhn(long card)
{
    // Create a reverse array of the card
    int *cardArr = longToArray(card);

    // Sum of the (sum of the digits of twice the even idx) and odd idx
    int sum = 0;
    for (int i = 0; i < longlen(card); i++)
    {
        if (i & 1)
        {
            sum += sumOfDigits(cardArr[i] * 2);
        }
        else
        {
            sum += cardArr[i];
        }
    }

    // Last digit is 0
    return (sum % 10 == 0);
}
