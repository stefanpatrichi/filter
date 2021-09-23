#include "bmp.h"

#include <climits>
#include <fstream>
#include <string>
#include <vector>

// helper functions

// border with 0
void border(const int height, const int width, std::vector<std::vector<BIGRGBTRIPLE>>& image) {
  for (int c = 0; c < width; ++c) image[0][c] = image[height - 1][c] = { 0, 0, 0 };
  for (int l = 0; l < height; ++l) image[l][0] = image[l][width - 1] = { 0, 0, 0 };
}

// btor = BIGRGBTRIPLE to RGBTRIPLE
// ! to be used with caution (potential overflow)
RGBTRIPLE btor(const BIGRGBTRIPLE colour) {
/*  if (colour.rgbtBlue > UCHAR_MAX || colour.rgbtGreen > UCHAR_MAX || colour.rgbtRed > UCHAR_MAX) 
    throw std::overflow_error("btor(): BIGRGBTRIPLE too big");*/
  return RGBTRIPLE{ colour.rgbtBlue, colour.rgbtGreen, colour.rgbtRed };
}

// ---------------- RGBTRIPLE ----------------

RGBTRIPLE::RGBTRIPLE()
  : rgbtBlue(0), rgbtGreen(0), rgbtRed(0) {}


RGBTRIPLE::RGBTRIPLE(BYTE blue, BYTE green, BYTE red)
  : rgbtBlue(blue), rgbtGreen(green), rgbtRed(red) {}

RGBTRIPLE::RGBTRIPLE(const RGBTRIPLE& other)
  : rgbtBlue(other.rgbtBlue), rgbtGreen(other.rgbtGreen), rgbtRed(other.rgbtRed) {}

RGBTRIPLE& RGBTRIPLE::operator=(const RGBTRIPLE& other) {
  if (this != &other) {
    rgbtBlue  = other.rgbtBlue;
    rgbtGreen = other.rgbtGreen;
    rgbtRed   = other.rgbtRed;
  }
  return *this;
}

// ---------------- BIGRGBTRIPLE ----------------

BIGRGBTRIPLE::BIGRGBTRIPLE()
  : rgbtBlue(0), rgbtGreen(0), rgbtRed(0) {}

BIGRGBTRIPLE::BIGRGBTRIPLE(WORD blue, WORD green, WORD red)
  : rgbtBlue(blue), rgbtGreen(green), rgbtRed(red) {}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator+=(const BIGRGBTRIPLE& rhs) {
  rgbtBlue  += rhs.rgbtBlue;
  rgbtGreen += rhs.rgbtGreen;
  rgbtRed   += rhs.rgbtRed;
  return *this;
}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator-=(const BIGRGBTRIPLE& rhs) {
  rgbtBlue  -= rhs.rgbtBlue;
  rgbtGreen -= rhs.rgbtGreen;
  rgbtRed   -= rhs.rgbtRed;
  return *this;
}

inline BIGRGBTRIPLE BIGRGBTRIPLE::operator+(const BIGRGBTRIPLE& rhs) {
  *this += rhs;
  return *this;
}

inline BIGRGBTRIPLE BIGRGBTRIPLE::operator-(const BIGRGBTRIPLE& rhs) {
  *this -= rhs;
  return *this;
}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator+=(const RGBTRIPLE& rhs) {
  rgbtBlue  += rhs.rgbtBlue;
  rgbtGreen += rhs.rgbtGreen;
  rgbtRed   += rhs.rgbtRed;
  return *this;
}

inline BIGRGBTRIPLE BIGRGBTRIPLE::operator+(const RGBTRIPLE& rhs) {
  *this += rhs;
  return *this;
}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator-=(const RGBTRIPLE& rhs) {
  rgbtBlue  -= rhs.rgbtBlue;
  rgbtGreen -= rhs.rgbtGreen;
  rgbtRed   -= rhs.rgbtRed;
  return *this;
}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator/=(const int& rhs) {
  rgbtBlue  /= rhs;
  rgbtGreen /= rhs;
  rgbtRed   /= rhs;
  return *this;
}

inline BIGRGBTRIPLE BIGRGBTRIPLE::operator/(const int& rhs) {
  *this /= rhs;
  return *this;
}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator/=(int&& rhs) {
  rgbtBlue  /= rhs;
  rgbtGreen /= rhs;
  rgbtRed   /= rhs;
  return *this;
}

inline BIGRGBTRIPLE BIGRGBTRIPLE::operator/(int&& rhs) {
  *this /= rhs;
  return *this;
}


// ---------------- BMPImage ----------------

BMPImage::BMPImage(const char* filename) { read(filename); }

void BMPImage::read(const char* filename) {
  int height, width, padding;
  std::ifstream fin(filename, std::ios::binary);
  if (!fin)
    throw std::runtime_error((std::string)"Nu s-a putut deschide " + (std::string)filename + (std::string)".");

  readHeaders(fin);
    
  // allocate memory for image
  height = abs(info_header.biHeight);
  width = info_header.biWidth;
  data.resize(height);
  for (auto& line : data)
    line.resize(width);
  
  padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
  //              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ can be negative

  for (auto& line : data) {
    fin.read((char*)line.data(), width * sizeof(RGBTRIPLE));
    fin.seekg(padding, std::ios::cur);  // skip over padding
  }
}

void BMPImage::write(const char* filename) {
  int width, padding;
  std::ofstream fout(filename, std::ios::binary);
  if (!fout)
    throw std::runtime_error((std::string)"Nu s-a putut deschide " + (std::string)filename + (std::string)".");

  writeHeaders(fout);

  width = info_header.biWidth;
  padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
  //              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ can be negative

  for (auto& line : data) {
    fout.write((char*)line.data(), width * sizeof(RGBTRIPLE));
    for (int i = 0; i < padding; ++i) fout.put(0);  // write padding at end of row  
  }
}

void BMPImage::blur(const int radius) {
  int height = abs(info_header.biHeight), width = info_header.biWidth;

  // box blur is a separable filter

  // horizontal blur

  // BIGRGBTRIPLE aux[height][width];
  std::vector<std::vector<BIGRGBTRIPLE>> aux(height, std::vector<BIGRGBTRIPLE>(width));

  for (int l = 0; l < height; ++l) {
    BIGRGBTRIPLE sum;  // initialised to 0 in ctor
    // first pixel will use sum from 0 to radius
    for (int c = 0; c < 2 * radius + 1; ++c) {
      sum += data[l][c];          // update sum
      aux[l][c] = sum / (c + 1);  // set current pixel to average until now (edge case)
    }

    // next windows
    // the only variable is first so instead of calculating last at each iter just increment
    for (int first = 1, last = 2 * radius + 1; first <= width - 2 * radius - 1; ++first, ++last) {
      // update sum
      sum += data[l][last];
      sum -= data[l][first - 1];
      // set pixel to average
      aux[l][first + radius] = sum / (2 * radius + 1);
    }
  }
/*
  // vertical blur
  for (int c = 0; c < width; ++c) {
    BIGRGBTRIPLE sum;
    // first window
    for (int l = 0; l < 2 * radius + 1; ++l) { 
      sum += data[l][c];
      aux[l][c] = sum / (l + 1);
    }

    // next windows
    for (int first = 1, last = 2 * radius + 1; first <= height - 2 * radius - 1; ++first, ++last) {
      sum += data[last][c];
      sum -= data[first - 1][c];
      aux[first + radius + 1][c] = sum / radius;
    }
  }
 */ 
  for (int l = 0; l < height; ++l)
    for (int c = 0; c < width; ++c)
      data[l][c] = btor(aux[l][c]);
}

void BMPImage::readHeaders(std::ifstream& fin) {
  fin.read((char*)&file_header, sizeof(file_header));
  fin.read((char*)&info_header, sizeof(info_header));
  if (file_header.bfType != 0x4d42 || file_header.bfOffBits != 54 || info_header.biSize != 40 || 
      info_header.biBitCount != 24 || info_header.biCompression != 0)
    throw std::runtime_error("Formatul de fisier nu este suportat. Fisierul este sigur BMP 24-bit?");
}

void BMPImage::writeHeaders(std::ofstream& fout) {
  fout.write((char*)&file_header, sizeof(file_header));
  fout.write((char*)&info_header, sizeof(info_header));
}

