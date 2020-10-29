#include "helpers.h"

#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float total = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            int prom = round(total / 3);
            image[i][j].rgbtBlue = prom;
            image[i][j].rgbtGreen = prom;
            image[i][j].rgbtRed = prom;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float blue, green, red;
    int finalblue, finalgreen, finalred;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;
            green = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;
            blue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;

            finalblue = round(blue);
            finalgreen = round(green);
            finalred = round(red);

            if (finalblue > 255)
            {
                finalblue = 255;
            }
            if (finalgreen > 255)
            {
                finalgreen = 255;
            }
            if (finalred > 255)
            {
                finalred = 255;
            }

            image[i][j].rgbtBlue = finalblue;
            image[i][j].rgbtGreen = finalgreen;
            image[i][j].rgbtRed = finalred;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            RGBTRIPLE store = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = store;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE store[3][3];
    RGBTRIPLE prom;
    RGBTRIPLE cero;
    cero.rgbtBlue = 0;
    cero.rgbtGreen = 0;
    cero.rgbtRed = 0;

    int pixels;
    float sumBlue, sumGreen, sumRed;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            if (i == 0 || j == 0)
            {
                store[0][0] = cero;
                store[0][1] = cero;
                store[0][2] = cero;
                store[1][0] = cero;
                store[1][1] = image[i][j];
                store[1][2] = image[i][j + 1];
                store[2][0] = cero;
                store[2][1] = image[i + 1][j];
                store[2][2] = image[i + 1][j + 1];
                pixels = 4;
            }
            else if (i == 0 || j == width - 1)
            {
                store[0][0] = cero;
                store[0][1] = cero;
                store[0][2] = cero;
                store[1][0] = image[i][j - 1];
                store[1][1] = image[i][j];
                store[1][2] = cero;
                store[2][0] = image[i + 1][j - 1];
                store[2][1] = image[i + 1][j];
                store[2][2] = cero;
                pixels = 4;
            }
            else if (i == height - 1 || j == 0)
            {
                store[0][0] = cero;
                store[0][1] = image[i - 1][j];
                store[0][2] = image[i - 1][j + 1];
                store[1][0] = cero;
                store[1][1] = image[i][j];
                store[1][2] = image[i][j + 1];
                store[2][0] = cero;
                store[2][1] = cero;
                store[2][2] = cero;
                pixels = 4;
            }
            else if (i == height - 1 || j == width - 1)
            {
                store[0][0] = image[i - 1][j - 1];
                store[0][1] = image[i - 1][j];
                store[0][2] = cero;
                store[1][0] = image[i][j - 1];
                store[1][1] = image[i][j];
                store[1][2] = cero;
                store[2][0] = cero;
                store[2][1] = cero;
                store[2][2] = cero;
                pixels = 4;
            }
            else if (i == 0)
            {
                store[0][0] = cero;
                store[0][1] = cero;
                store[0][2] = cero;
                store[1][0] = image[i][j - 1];
                store[1][1] = image[i][j];
                store[1][2] = image[i][j + 1];
                store[2][0] = image[i + 1][j - 1];
                store[2][1] = image[i + 1][j];
                store[2][2] = image[i + 1][j + 1];
                pixels = 6;
            }
            else if (i == height - 1)
            {
                store[0][0] = image[i - 1][j - 1];
                store[0][1] = image[i - 1][j];
                store[0][2] = image[i - 1][j + 1];
                store[1][0] = image[i][j - 1];
                store[1][1] = image[i][j];
                store[1][2] = image[i][j + 1];
                store[2][0] = cero;
                store[2][1] = cero;
                store[2][2] = cero;
                pixels = 6;
            }
            else if (j == 0)
            {
                store[0][0] = cero;
                store[0][1] = image[i - 1][j];
                store[0][2] = image[i - 1][j + 1];
                store[1][0] = cero;
                store[1][1] = image[i][j];
                store[1][2] = image[i][j + 1];
                store[2][0] = cero;
                store[2][1] = image[i + 1][j];
                store[2][2] = image[i + 1][j + 1];
                pixels = 6;
            }
            else if (j == width - 1)
            {
                store[0][0] = image[i - 1][j - 1];
                store[0][1] = image[i - 1][j];
                store[0][2] = cero;
                store[1][0] = image[i][j - 1];
                store[1][1] = image[i][j];
                store[1][2] = cero;
                store[2][0] = image[i + 1][j - 1];
                store[2][1] = image[i + 1][j];
                store[2][2] = cero;
                pixels = 6;
            }
            else
            {
                store[0][0] = image[i - 1][j - 1];
                store[0][1] = image[i - 1][j];
                store[0][2] = image[i - 1][j + 1];
                store[1][0] = image[i][j - 1];
                store[1][1] = image[i][j];
                store[1][2] = image[i][j + 1];
                store[2][0] = image[i + 1][j - 1];
                store[2][1] = image[i + 1][j];
                store[2][2] = image[i + 1][j + 1];
                pixels = 9;
            }

            sumBlue = 0;
            sumGreen = 0;
            sumRed = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int h = 0; h < 3; h++)
                {
                    sumBlue = sumBlue + store[k][h].rgbtBlue;
                    sumGreen = sumGreen + store[k][h].rgbtGreen;
                    sumRed = sumRed + store[k][h].rgbtRed;
                }
            }

            prom.rgbtBlue = round(sumBlue / pixels);
            prom.rgbtGreen = round(sumGreen / pixels);
            prom.rgbtRed = round(sumRed / pixels);

            image[i][j] = prom;
        }
    }
    return;
}
