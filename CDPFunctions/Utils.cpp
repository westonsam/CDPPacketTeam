#include "Utils.h"
#include <random>
#include <cfloat>
namespace duckutils {

void getRandomBytes(int length, uint8_t* bytes) {
  const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  ;
  int i;
  for (i = 0; i < length; i++) {
    bytes[i] = digits[getRandomNumber(36)];    
  }
}
int getRandomNumber(int range) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1, std::nextafter(range, DBL_MAX));

    for (int i=0; i<16; ++i) {
        std::cout << dist(mt) << "\n";
    }
    int num = dist(mt);
    return num;
}

// Note: This function is not thread safe
std::string convertToHex(uint8_t* data, int size) {
  std::string buf = ""; // static to avoid memory leak
  buf.clear();
  buf.reserve(size * 2); // 2 digit hex
  const char* cs = "0123456789ABCDEF";
  for (int i = 0; i < size; i++) {
    uint8_t val = data[i];
    buf += cs[(val >> 4) & 0x0F];
    buf += cs[val & 0x0F];
  }
  return buf;
}