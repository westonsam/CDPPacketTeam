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
        std::vector<uint8_t> convertStringToVector (std::string line);
        std::string convertVectorToString (std::vector<uint8_t> buffer);
        std::vector <uint8_t> convertNumToVector(uint32_t data);
        //int printVector(std::vector<uint8_t> vec, uint32_t size);
}