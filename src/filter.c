#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[]) {
  // allowable filters
  char *filters = "bgr";

  // get filter flag, check validity
  char filter = getopt(argc, argv, filters);
  if (filter == '?') {
    fprintf(stderr, "Invalid filter.\n");
    return 1;
  }

  // ensure only one filter
  if (getopt(argc, argv, filters) != -1) {
    fprintf(stderr, "Only one filter allowed.\n");
    return 2;
  }

  // ensure proper usage
  if (argc != optind + 2) {
    fprintf(stderr, "Usage: filter [flag] infile outfile\n");
    return 3;
  }

  char *infile = argv[optind];
  char *outfile = argv[optind + 1];

  FILE *inptr = fopen(infile, "r");
  if (inptr == NULL) {
    fprintf(stderr, "Could not open %s.\n", infile);
    return 4;
  }

  FILE *outptr = fopen(outfile, "w");
  if (outptr == NULL) {
    fclose(inptr);
    fprintf(stderr, "Could not create %s.\n", outfile);
    return 5;
  }

  // read infile's BITMAPFILEHEADER
  BITMAPFILEHEADER bf;
  fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

  // read infile's BITMAPINFOHEADER
  BITMAPINFOHEADER bi;
  fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

  // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
  if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
      bi.biBitCount != 24 || bi.biCompression != 0) {
    fclose(outptr);
    fclose(inptr);
    fprintf(stderr, "Unsupported file format.\n");
    return 6;
  }

  const int height = abs(bi.biHeight);
  const int width = bi.biWidth;

  // allocate memory for image
  RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
  if (image == NULL) {
    fprintf(stderr, "Not enough memory to store image.\n");
    fclose(outptr);
    fclose(inptr);
    return 7;
  }

  // determine padding for scanlines
  const int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

  for (int i = 0; i < height; i++) {
    fread(image[i], sizeof(RGBTRIPLE), width, inptr);

    // skip over padding
    fseek(inptr, padding, SEEK_CUR);
  }

  switch (filter) {
    case 'b':
      blur(height, width, image);
      break;

    case 'g':
      grayscale(height, width, image);
      break;

    case 'r':
      reflect(height, width, image);
      break;
  }

  // write outfile's BITMAPFILEHEADER
  fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

  // write outfile's BITMAPINFOHEADER
  fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

  // write new pixels to outfile
  for (int i = 0; i < height; i++) {
    fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

    // write padding at end of row
    for (int k = 0; k < padding; k++) fputc(0x00, outptr);
  }

  free(image);
  fclose(inptr);
  fclose(outptr);

  return 0;
}
