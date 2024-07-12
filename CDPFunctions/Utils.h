#include <iostream>
#include <fstream> //for file handling
#include <cstdlib> // for exit()
#include <string>
#include <cstdint> //used for uint8_t
#include <vector>  //includes vectors, similar arrays

namespace duckutils
{
        void getRandomBytes(int length, uint8_t *bytes);
        int getRandomNumber(int range);
        std::string convertToHex(uint8_t *data, int size);
        uint32_t toUint32(const uint8_t *data);
}