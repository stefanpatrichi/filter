#include <fstream>
#include <iostream>
#include <stdexcept>

#include "bmp.h"

static const int DEFAULT_RADIUS = 3;

int main(int argc, const char* argv[]) {
  std::ios::sync_with_stdio(0);
  int radius = DEFAULT_RADIUS;

  if (argc != 3 && argc != 4) 
    throw std::runtime_error( 
      "\nFolosire: ./filter imagine_intrare.bmp imagine_iesire.bmp [raza] (*nix)\n"
      "          filter.exe imagine_intrare.bmp imagine_iesire.bmp [raza] (Windows)\n"
    );
  
  if (argc == 4) {
    try {
      radius = std::stoi(argv[argc - 1]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Raza invalida\n";
    } catch (std::out_of_range& e) {
      std::cerr << "Raza in afara limitelor admise\n";
    }
    --argc;
  }

  // read image
  BMPImage image(argv[argc - 2]);
  image.write(argv[argc - 1]);

  return 0;
}
