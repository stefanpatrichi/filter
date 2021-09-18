#include "bmp.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// ---------------- RGBTRIPLE ----------------

RGBTRIPLE::RGBTRIPLE()
  : rgbtBlue(0), rgbtGreen(0), rgbtRed(0) {}


RGBTRIPLE::RGBTRIPLE(BYTE blue, BYTE green, BYTE red)
  : rgbtBlue(blue), rgbtGreen(green), rgbtRed(red) {}

RGBTRIPLE::RGBTRIPLE(const RGBTRIPLE& other)
  : rgbtBlue(other.rgbtBlue), rgbtGreen(other.rgbtGreen), rgbtRed(other.rgbtRed) {}

RGBTRIPLE& RGBTRIPLE::operator=(RGBTRIPLE rhs) {
  using std::swap;
  swap(*this, rhs);
  return *this;
}

RGBTRIPLE& RGBTRIPLE::operator+=(const RGBTRIPLE& rhs) {
  rgbtBlue  += rhs.rgbtBlue;
  rgbtGreen += rhs.rgbtGreen;
  rgbtRed   += rhs.rgbtRed;
  return *this;
}

RGBTRIPLE& RGBTRIPLE::operator-=(const RGBTRIPLE& rhs) {
  rgbtBlue  -= rhs.rgbtBlue;
  rgbtGreen -= rhs.rgbtGreen;
  rgbtRed   -= rhs.rgbtRed;
  return *this;
}

inline RGBTRIPLE RGBTRIPLE::operator+(const RGBTRIPLE& rhs) {
  *this += rhs;
  return *this;
}

inline RGBTRIPLE RGBTRIPLE::operator-(const RGBTRIPLE& rhs) {
  *this -= rhs;
  return *this;
}

// ---------------- BIGRGBTRIPLE ----------------

BIGRGBTRIPLE::BIGRGBTRIPLE()
  : rgbtBlue(0), rgbtGreen(0), rgbtRed(0) {}

BIGRGBTRIPLE::BIGRGBTRIPLE(WORD blue, WORD green, WORD red)
  : rgbtBlue(blue), rgbtGreen(green), rgbtRed(red) {}

BIGRGBTRIPLE::BIGRGBTRIPLE(const BIGRGBTRIPLE& other)
  : rgbtBlue(other.rgbtBlue), rgbtGreen(other.rgbtGreen), rgbtRed(other.rgbtRed) {}

BIGRGBTRIPLE& BIGRGBTRIPLE::operator=(BIGRGBTRIPLE rhs) {
  using std::swap;
  swap(*this, rhs);
  return *this;
}

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
  for (auto &line : data)
    line.resize(width);
  
  padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
  //              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ can be negative

  for (auto &line : data) {
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

  for (auto &line : data) {
    fout.write((char*)line.data(), width * sizeof(RGBTRIPLE));
    for (int i = 0; i < padding; ++i) fout.put(0);  // write padding at end of row  
  }
}

void BMPImage::blur() {
  // TODO
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
