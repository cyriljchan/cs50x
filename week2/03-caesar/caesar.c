#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype
int isNumber(string text);
string caesar(string text, int key);

int main(int argc, string argv[])
{
    // Check argument count
    if (argc != 2)
    {
        printf("Usage: ./caesar [key]\n");
        return 1;
    }

    // Check key is numeric
    else if (!isNumber(argv[1]))
    {
        printf("Error: key must be numeric\n");
        return 1;
    }

    else
    {
        string text = get_string("plaintext: ");
        string cipher = caesar(text, atoi(argv[1]));
        printf("ciphertext: %s\n", cipher);
    }
}

int isNumber(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        if (!isdigit(text[i]))
        {
            return 0;
        }
    }
    return 1;
}

string caesar(string text, int key)
{
    // Rotate back to 1 if key exceeds 26
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            if (isupper(text[i]))
            {
                text[i] = (text[i] - 65 + key % 26) % 26 + 65;
            }
            else
            {
                text[i] = (text[i] - 97 + key % 26) % 26 + 97;
            }
        }
    }

    return text;
}
