#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute for Coleman-Liau index
    double L = letters * 100.00 / words;
    double S = sentences * 100.00 / words;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (1 > index)
    {
        printf("Before Grade 1\n");
    }
    else if (16 > index)
    {
        printf("Grade %d\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

// Count letters in the text
int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // Look for alphanumeric characters
        if (isalpha(text[i]))
        {
            letters += 1;
        }
    }
    return letters;
}

// Count words in text
int count_words(string text)
{
    int words = 0;
    // Adjust for the first word
    if (strlen(text) != 0)
    {
        words += 1;
    }
    for (int i = 0; i < strlen(text); i++)
    {
        // Look for spaces
        if (isspace(text[i]))
        {
            words += 1;
        }
    }
    return words;
}

// Count sentences in text
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // Look for '.', '!', or '?'
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences += 1;
        }
    }
    return sentences;
}
