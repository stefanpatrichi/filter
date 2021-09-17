#include "bmp.h"

#include <fstream>
#include <vector>

// ---------------- RGBTRIPLE ----------------

RGBTRIPLE::RGBTRIPLE(int blue, int green, int red)
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

// ---------------- BMPImage ----------------

BMPImage::BMPImage(const char* filename) { read(filename); }

BMPImage::BMPImage(int height, int width) {
  // TODO
}

void BMPImage::read(const char* filename) {
  std::ifstream fin(filename, std::ios::binary);
  // TODO
}

void BMPImage::write(const char* filename) {
  std::ofstream fout(filename, std::ios::binary);
  // TODO
}

void BMPImage::blur() {
  // TODO
}

void BMPImage::readHeaders(std::ifstream& fin) {
  // TODO
}

void BMPImage::writeHeaders(std::ofstream& fout) {
  // TODO
}
