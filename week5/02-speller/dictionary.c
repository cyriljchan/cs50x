// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 5000;

// Hash table
node *table[N];

// Count of words in the dictionary
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int key = hash(word);
    node *ptr = table[key];
    while (ptr != NULL)
    {
        if (!strcasecmp(word, ptr->word))
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int key = 0;
    for (int i=0; i<strlen(word); i++)
    {
        key += toupper(word[i]);
    }
    return key % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *d = fopen(dictionary, "r");
    if (d == NULL)
    {
        return false;
    }

    // While not EOF
    char word[LENGTH + 1];
    while (fgets(word, LENGTH + 2, d)) // LENGTH+2 since fget() reads buffer-1 then moves to the next chars
    {
        // Preprocess node and word
        word[strcspn(word, "\n")] = '\0'; // Replace \n with \0
        node *n = malloc(sizeof(node));
        strcpy(n->word, word);

        // Prepend to table[key]
        int key = hash(n->word);
        n->next = table[key];
        table[key] = n;
        word_count++;
    }
    fclose(d);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }
    return true;
}
