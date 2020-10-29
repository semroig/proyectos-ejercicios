// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 2750;

// Hash table
node *table[N];

int dict = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int place = hash(word);
    int compare;
    node *cursor = table[place];
    while (cursor)
    {
        compare = strcasecmp(cursor->word, word);
        if (compare == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        char low = tolower(word[i]);
        hash = 31 * hash + low;
    }
    return hash % 2750;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *ptr = fopen(dictionary, "r");
    if (!ptr)
    {
        return false;
    }

    int final = 1;
    int code;

    char copy[45];

    while(final != EOF)
    {
        final = fscanf(ptr, "%s", copy);

        node *gancho = malloc(sizeof(node));
        if (!gancho)
        {
            dict = 0;
            fclose(ptr);
            return false;
        }
        strcpy(gancho->word, copy);
        gancho->next = NULL;

        code = hash(copy);

        gancho->next = table[code];
        table[code] = gancho;
        dict++;
    }
    fclose(ptr);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (dict != 0)
    {
        return dict - 1;
    }
    return dict;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < 2750; i++)
    {
        node *cursor = table[i];
        node *tmp = NULL;
        while (cursor)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
