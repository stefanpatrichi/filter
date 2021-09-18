#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

/**
 * common data types
 * 
 * https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/efda8314-6e41-4837-8299-38ba0ee04b92
 */
using BYTE  = uint8_t;
using DWORD = uint32_t;
using LONG  = int32_t;
using WORD  = uint16_t;

/**
 * a BITMAPFILEHEADER contains info about the type, size, layout of a BMP file
 *
 * https://docs.microsoft.com/en-gb/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
 */
struct BITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} __attribute__((__packed__));

/**
 * a BITMAPINFOHEADER contains info about the dimensions and colour size of a BMP file
 *
 * https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
 */
struct BITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} __attribute__((__packed__));

/**
 * RGBTRIPLE describes a colour consisting of relative intensities of red, green and blue.
 *
 * https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-rgbtriple
 */
struct RGBTRIPLE {
  BYTE rgbtBlue;
  BYTE rgbtGreen;
  BYTE rgbtRed;

  RGBTRIPLE();
  RGBTRIPLE(BYTE blue, BYTE green, BYTE red);
  RGBTRIPLE(const RGBTRIPLE&);
  RGBTRIPLE& operator=(RGBTRIPLE);

  RGBTRIPLE& operator+=(const RGBTRIPLE&);
  RGBTRIPLE& operator-=(const RGBTRIPLE&);
  RGBTRIPLE operator+(const RGBTRIPLE&);
  RGBTRIPLE operator-(const RGBTRIPLE&);
} __attribute__((__packed__));

struct BIGRGBTRIPLE {
  WORD rgbtBlue;
  WORD rgbtGreen;
  WORD rgbtRed;

  BIGRGBTRIPLE();
  BIGRGBTRIPLE(WORD blue, WORD green, WORD red);
  BIGRGBTRIPLE(const BIGRGBTRIPLE&);
  BIGRGBTRIPLE& operator=(BIGRGBTRIPLE);

  BIGRGBTRIPLE& operator+=(const BIGRGBTRIPLE&);
  BIGRGBTRIPLE& operator-=(const BIGRGBTRIPLE&);
  BIGRGBTRIPLE operator+(const BIGRGBTRIPLE&);
  BIGRGBTRIPLE operator-(const BIGRGBTRIPLE&);
} __attribute__((__packed__));



class BMPImage {
public:
  BMPImage(const char* filename);

  BMPImage(BMPImage&&)            = delete;
  BMPImage(const BMPImage&)       = delete;
  void operator=(BMPImage&&)      = delete;
  void operator=(const BMPImage&) = delete;

  void read(const char* filename);
  void write(const char* filename);
  void blur();
private:
  BITMAPFILEHEADER file_header;
  BITMAPINFOHEADER info_header;
  std::vector<std::vector<RGBTRIPLE>> data;

  void readHeaders(std::ifstream&);
  void writeHeaders(std::ofstream&);
};
