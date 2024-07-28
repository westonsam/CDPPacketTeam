#include "unity.h"
#include "Utils.h"
#include <iostream>
#include <cstring>

void setUp(void) {
}

void tearDown(void) {

}

void test_getRandomBytes(void) {
    int length = 10;
    uint8_t bytes[length];
    memset(bytes, 0, length); // Initialize the array with zeros

    duckutils::getRandomBytes(length, bytes);
    std::cout << "Random array: " << bytes << std::endl;
    for (int i = 0; i < length; i++) {
        TEST_ASSERT_TRUE((bytes[i] >= '0' && bytes[i] <= '9') ||
                         (bytes[i] >= 'A' && bytes[i] <= 'Z'));
    }
}

void test_getRandomNumber(void) {
    int range = 36;
    for (int i = 0; i < 100; i++) { // Test 100 times
        int num = duckutils::getRandomNumber(range);
        TEST_ASSERT_TRUE(num >= 1 && num <= range);
    }
}

void test_convertToHex(void) {
    uint8_t data[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    std::string expected = "0123456789ABCDEF";
    std::string result = duckutils::convertToHex(data, sizeof(data));

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), result.c_str());
}

void test_toUint32(void) {
    uint8_t data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint32_t expected = 0xDEADBEEF;
    uint32_t result = duckutils::toUint32(data);

    TEST_ASSERT_EQUAL_UINT32(expected, result);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_getRandomBytes);
    RUN_TEST(test_getRandomNumber);
    RUN_TEST(test_convertToHex);
    RUN_TEST(test_toUint32);

    UNITY_END();
    return 0;
}

