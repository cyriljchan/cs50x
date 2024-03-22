#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// PROTOTYPE
int check_all_alpha(string key);
int check_dup(string text);
string to_all_upper(string text);
string encrypt_text(string text, string key);

int main(int argc, string argv[])
{
    // Check number of arguments
    if (argc == 2)
    {
        // Check key length is 26 and is alphabets and is all unique
        if (strlen(argv[1]) == 26 && check_all_alpha(argv[1]) && !check_dup(argv[1]))
        {
            string text = get_string("plaintext: ");
            string cipher = encrypt_text(text, to_all_upper(argv[1]));
            printf("ciphertext: %s\n", cipher);
        }
        else
        {
            printf("Argument error: Key must contain 26 unique letters.\n");
            return 1;
        }
    }
    else
    {
        printf("Missing argument: Expected ./substitution [key]\n");
        return 1;
    }
}

// Checks if string is all alphabet
int check_all_alpha(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        if (!isalpha(text[i]))
        {
            return 0;
        }
    }
    return 1;
}

// Check for duplicate letters in string
int check_dup(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        for (int j = i + 1; j < strlen(text); j++)
        {
            if (text[i] == text[j])
            {
                return 1;
            }
        }
    }
    return 0;
}

// Changes all lowercase to uppercase
string to_all_upper(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        if (islower(text[i]))
        {
            text[i] = toupper(text[i]);
        }
    }
    return text;
}

// Encrypts text through desired key
string encrypt_text(string text, string key)
{
    for (int i = 0; i < strlen(text); i++)
    {
        for (int j = 0; j < strlen(key); j++)
        {
            // j + 65 for uppercase letter
            if (toupper(text[i]) == j + 65)
            {
                // cipher = letter + (distance between key and letter)
                text[i] = text[i] + (key[j] - (j + 65));
                break;
            }
        }
    }
    return text;
}
