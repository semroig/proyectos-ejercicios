//I include the libraries i will need
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//I declare a global array
char keys[2][26];

//I prototype my function
void cipher(string s);

int main(int argc, string argv[])
{
    //I store the argument as the key
    string key = argv[1];

    if (argc != 2)
    {
        //It means that the user entered more than one argument
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(key) != 26)
    {
        //It means that the key given is shorter or longer than it should be
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //I loop over each char of the key
    for (int i = 0; i < 26; i++)
    {
        char letter = key[i];

        //I check if the char is a letter or something else
        bool is_alpha = isalpha(letter);
        if (is_alpha == false)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }

        //I store the letter in the array as lowercase and uppercase
        keys[0][i] = tolower(letter);
        keys[1][i] = toupper(letter);

        //I loop over the other letters of the key to compare them, looking for duplicates
        for (int z = i - 1; z >= 0; z = z - 1)
        {
            if (keys[0][i] == keys[0][z])
            {
                //It means its duplicated
                printf("Key must contain 26 DIFFERENT characters.\n");
                return 1;
            }
        }
    }

    //I ask the user for the text to cipher
    string plaintext = get_string("plaintext:  ");

    //I print the chipered text
    printf("ciphertext: ");
    cipher(plaintext);
    printf("\n");
    return 0;
}

void cipher(string s)
{
    int lenght = strlen(s);
    for (int i = 0; i < lenght; i++)
    {
        char letter = s[i];

        //I check if the letter is lowercase or uppercase
        bool is_lower = islower(letter);
        bool is_upper = isupper(letter);
        if (is_lower == true)
        {
            //Using ASCII i relate the letter with the place order of the key letters
            int key_place = (int) letter - 97;
            printf("%c", keys[0][key_place]);
        }
        else if (is_upper == true)
        {
            //Using ASCII i relate the letter with the place order of the key letters
            int key_place = (int) letter - 65;
            printf("%c", keys[1][key_place]);
        }
        else
        {
            //If is not a letter, i print it without modifications
            printf("%c", letter);
        }
    }
}