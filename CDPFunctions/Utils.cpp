#include "Utils.h"
#include <random>
#include <cfloat>
#include <vector>
#include <string>
#include <cstdint>



namespace duckutils
{

  void getRandomBytes(int length, uint8_t *bytes)
  {
    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    ;
    int i;
    for (i = 0; i < length; i++)
    {
      bytes[i] = digits[getRandomNumber(36)];
    }
  }
  int getRandomNumber(int range)
  {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1, std::nextafter(range, DBL_MAX));

    for (int i = 0; i < 16; ++i)
    {
      //std::cout << dist(mt) << "\n";
    }
    int num = dist(mt);
    return num;
  }

  // Note: This function is not thread safe
  std::string convertToHex(uint8_t *data, int size)
  {
    std::string buf = ""; // static to avoid memory leak
    buf.clear();
    buf.reserve(size * 2); // 2 digit hex
    const char *cs = "0123456789ABCDEF";
    for (int i = 0; i < size; i++)
    {
      uint8_t val = data[i];
      buf += cs[(val >> 4) & 0x0F];
      buf += cs[val & 0x0F];
    }
    return buf;
  }

  uint32_t toUint32(const uint8_t *data)
  {
    uint32_t value = 0;

    value |= data[0] << 24;
    value |= data[1] << 16;
    value |= data[2] << 8;
    value |= data[3];
    return value;
  }
  
  std::vector<uint8_t> convertStringToVector (std::string line) 
  {
    
    std::vector<uint8_t> buffer;
    for (char str_char : line)
        buffer.push_back(uint8_t(str_char));
    return buffer;
  } 


  std::string convertVectorToString (std::vector<uint8_t> buffer) 
  {
      std::string str = "";
      for (int i = 0; i < buffer.size(); i++) {
          str += buffer[i];
      }
      return str;
  }

  std::vector <uint8_t> convertNumToVector(uint32_t data) 
  {
    std::vector<uint8_t> newData;
    for(int i=0; i < 4; i++)
    {
      newData.push_back(data >> (24-(i*8)) & 0xFF);
    }
    return newData;
  }
  /*int printVector(std::vector<uint8_t> vec, uint32_t size)
  {
    for (int i = 0; i < size; i++)
    {
      std::cout << "index: " << i << " data " << vec[i] << std::endl;
    }
    return 0;
  }*/
}