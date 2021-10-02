#include "bmp.h"

#include <math.h>

static const int RANGE = 6;

// RGBTRIPLE used for sum to avoid overflows
typedef struct {
  short rgbtBlue;
  short rgbtGreen;
  short rgbtRed;
} __attribute__((__packed__)) BIGRGBTRIPLE;

// swap two RGBTRIPLEs
void swap(RGBTRIPLE *x, RGBTRIPLE *y) {
  RGBTRIPLE aux = *x;
  *x = *y;
  *y = aux;
}

// convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
  for (int l = 0; l < height; ++l) {
    for (int c = 0; c < width; ++c) {
      // set all three colours to their average
      image[l][c].rgbtBlue = image[l][c].rgbtGreen = image[l][c].rgbtRed =
          round((float)(image[l][c].rgbtBlue + image[l][c].rgbtGreen + image[l][c].rgbtRed) / 3);
    }
  }
}

// reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
  for (int l = 0; l < height; ++l) {
    // loop through half of the columns
    for (int c = 0; c < width / 2 + width % 2; ++c) {
      // swap current pixel and the one symmetrical to current
      swap(&image[l][c], &image[l][width - 1 - c]);
    }
  }
}

// blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE aux[height][width];  // matrix to perform calculations on
  BIGRGBTRIPLE sum;

  for (int l = 0; l < height; ++l) {
    for (int c = 0; c < width; ++c) {
      // initialise sum and counter
      sum.rgbtBlue = 0;
      sum.rgbtGreen = 0;
      sum.rgbtRed = 0;
      int count = 0;

      // check surroundings
      for (int k = -RANGE / 2; k <= RANGE / 2; ++k) {
        for (int h = -RANGE / 2; h <= RANGE / 2; ++h) {
          // check if pixel in inside
          if (l + k >= 0 && l + k < height && c + h >= 0 && c + h < width) {
            sum.rgbtBlue += image[l + k][c + h].rgbtBlue;
            sum.rgbtGreen += image[l + k][c + h].rgbtGreen;
            sum.rgbtRed += image[l + k][c + h].rgbtRed;
            ++count;
          }
        }
      }

      // average
      aux[l][c].rgbtBlue = round((float)sum.rgbtBlue / count);
      aux[l][c].rgbtGreen = round((float)sum.rgbtGreen / count);
      aux[l][c].rgbtRed = round((float)sum.rgbtRed / count);
    }
  }

  // copy from aux
  for (int l = 0; l < height; ++l) {
    for (int c = 0; c < width; ++c) {
      image[l][c].rgbtBlue = aux[l][c].rgbtBlue;
      image[l][c].rgbtGreen = aux[l][c].rgbtGreen;
      image[l][c].rgbtRed = aux[l][c].rgbtRed;
    }
  }
}