#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// PROTOTYPE
void lowerword(string word);
int scorecheck(string word);

int main()
{
    string word1 = get_string("Player 1: ");
    lowerword(word1);
    string word2 = get_string("Player 2: ");
    lowerword(word2);

    if (scorecheck(word1) > scorecheck(word2))
    {
        printf("Player 1 wins!\n");
    }
    else if (scorecheck(word1) < scorecheck(word2))
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// Lowercase the passed word
void lowerword(string word)
{
    for (int i = 0; i < strlen(word); i++)
    {
        if (isupper(word[i]))
        {
            word[i] = tolower(word[i]);
        }
    }
}

// Score check
int scorecheck(string word)
{
    string letters = "aeilnorstudgbcmpfhvwykjxqz"; // letters sorted by score
    int score = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        for (int j = 0; j < strlen(letters); j++)
        {
            if (word[i] == letters[j])
            {
                if (j < 10)
                {
                    score += 1;
                }
                else if (j < 12)
                {
                    score += 2;
                }
                else if (j < 16)
                {
                    score += 3;
                }
                else if (j < 21)
                {
                    score += 4;
                }
                else if (j < 22)
                {
                    score += 5;
                }
                else if (j < 24)
                {
                    score += 8;
                }
                else if (j < 26)
                {
                    score += 10;
                }
            }
        }
    }
    return score;
}
