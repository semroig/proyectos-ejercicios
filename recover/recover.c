#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    //I declare my variables, for conteo i ask extra space to avoid segmentation fault
    unsigned char *array = malloc(sizeof(unsigned char) * 512);
    char *conteo = malloc(sizeof(char) * 100);
    int i = 0;
    int space = 512;

    //Check if command line argument is given
    if (argc != 2)
    {
        printf("Please, execute with exactly one command-line argument\n");
        return 1;
    }

    //I open the given file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("The given image cannot be opened for reading\n");
        return 1;
    }

    FILE *image;

    while (space == 512)
    {
        //I read a block of 512 bytes
        space = fread(array, 1, 512, file);

        //I check if it is a jpeg starting block
        if (array[0] == 0xff && array[1] == 0xd8 && array[2] == 0xff && (array[3] & 0xf0) == 0xe0)
        {
            if (i == 0)
            {
                image = fopen("000.jpg", "a");
                fwrite(array, 1, 512, image);
                i++;
            }
            else
            {
                fclose(image);
                sprintf(conteo, "%03i.jpg", i);
                image = fopen(conteo, "a");
                fwrite(array, 1, 512, image);
                i++;
            }
        }
        else if (space < 512)
        {
            fwrite(array, 1, space, image);
        }
        else
        {
            fwrite(array, 1, 512, image);
        }
    }

    //I close de files and free the memory asked on the heap
    fclose(image);
    fclose(file);
    free(image);
    free(conteo);
}
