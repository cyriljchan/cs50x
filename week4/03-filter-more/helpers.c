#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Averaging the BGR Values to grayscale
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2.00); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the image to maintain original values
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // 3x3 Average BGR per pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Add and average each of the 3x3 around the target pixel
            int sum_blue = 0, sum_green = 0, sum_red = 0;
            float n = 0.0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                    {
                        sum_blue += copy[i + k][j + l].rgbtBlue;
                        sum_green += copy[i + k][j + l].rgbtGreen;
                        sum_red += copy[i + k][j + l].rgbtRed;
                        n += 1.0;
                    }
                }
            }
            image[i][j].rgbtBlue = round(sum_blue / n);
            image[i][j].rgbtGreen = round(sum_green / n);
            image[i][j].rgbtRed = round(sum_red / n);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the image to maintain original values
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Sobel filter per pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute for Gx
            int x_blue = 0, x_green = 0, x_red = 0, two = 1;
            // Using -1:1 as offsets for the 3x3
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int x = i + k, y = j + l; // x and y as the 3x3 direction
                    if (x > -1 && x < height && y > -1 && y < width)
                    {
                        // "two" as the "*2" factor of the middle pixels
                        if (k == 0)
                        {
                            two = 2;
                        }
                        x_blue += copy[x][y].rgbtBlue * l * two;
                        x_green += copy[x][y].rgbtGreen * l * two;
                        x_red += copy[x][y].rgbtRed * l * two;
                        two = 1;
                    }
                }
            }

            // Compute for Gy
            int y_blue = 0, y_green = 0, y_red = 0;
            two = 1;
            // Using -1:1 as offsets for the 3x3
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int x = i + k, y = j + l; // x and y as the 3x3 direction
                    if (x > -1 && x < height && y > -1 && y < width)
                    {
                        // "two" as the "*2" factor of the middle pixels
                        if (l == 0)
                        {
                            two = 2;
                        }
                        y_blue += copy[x][y].rgbtBlue * k * two;
                        y_green += copy[x][y].rgbtGreen * k * two;
                        y_red += copy[x][y].rgbtRed * k * two;
                        two = 1;
                    }
                }
            }

            // Sobel filter algorithm
            image[i][j].rgbtGreen = fmin(fmax(0, round(sqrt(pow(x_green, 2) + pow(y_green, 2)))), 255);
            image[i][j].rgbtBlue = fmin(fmax(0, round(sqrt(pow(x_blue, 2) + pow(y_blue, 2)))), 255);
            image[i][j].rgbtRed = fmin(fmax(0, round(sqrt(pow(x_red, 2) + pow(y_red, 2)))), 255);
        }
    }
    return;
}
