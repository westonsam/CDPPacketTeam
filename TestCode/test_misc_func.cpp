#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include "unity.h"
#include <iostream>
#include <ostream>
using std::string;
using std::vector;

vector<uint8_t> convertStringToVector (string line) {

    vector<uint8_t> buffer;
    for (char str_char : line)
        buffer.push_back(static_cast<uint8_t>(str_char));
    return buffer;
}


string convertVectorToString (vector<uint8_t> buffer) {
    string str = "";
    for (int i = 0; i < buffer.size(); i++) {
        str += buffer[i];
    }
    return str;
}

//Test cases

void setUp(void) {}
void tearDown(void) {}


void test_convertStringToVector(void) {
    std::string input = "Testing FUNC1";
    uint8_t expected[] = { 'T','e','s','t','i','n','g',' ','F','U','N','C','1'};
    std::vector<uint8_t> result = convertStringToVector(input);

    std::cout << "Expected: ";
    for (auto elem : expected) {
        std::cout << static_cast<int>(elem) << " ";
    }
    std::cout << "Result: ";
    for (auto elem : result) {
        std::cout << static_cast<int>(elem) << " ";
    }
    std::cout << std::endl;


    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, result.data(), result.size());
}



void test_convertVectorToString(void) {
    uint8_t input[] = {'T', 'e', 's', 't', 'i', 'n', 'g', ' ', 'F', 'U', 'N', 'C', '2'};
    const char* expected = "Testing FUNC2";
    std::vector<uint8_t> vec_input(input, input + sizeof(input) / sizeof(input[0]));
    std::string result = convertVectorToString(vec_input);

    TEST_ASSERT_EQUAL_STRING(expected, result.c_str());
}

int main() {
    UNITY_BEGIN();

    // Run the tests
    RUN_TEST(test_convertStringToVector);
    RUN_TEST(test_convertVectorToString);

    return 0;
    UNITY_END();
}

