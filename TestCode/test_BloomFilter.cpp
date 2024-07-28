#include <memory>
#include "BloomFilter.h"
#include "unity.h"

    // Test case for BloomFilter constructor
    void
    test_bloomfilter_constructor(void) {
  BloomFilter bf(10, 5, 16, 50);
  TEST_ASSERT_EQUAL(10, bf.get_numSectors());
  TEST_ASSERT_EQUAL(5, bf.get_numHashes());
  TEST_ASSERT_EQUAL(16, bf.get_bitsPerSector());
  TEST_ASSERT_EQUAL(50, bf.get_maxMsgs());
  TEST_ASSERT_EQUAL(0, bf.get_nMsg());
}

// Test case for BloomFilter::bloom_add
void test_bloomfilter_bloom_add(void) {
  BloomFilter bf(10, 5, 16, 50);
  unsigned char msg[] = "ABCDE";
  int msgSize = sizeof(msg) - 1;  // exclude null terminator
  bf.bloom_add(msg, msgSize);
  TEST_ASSERT_EQUAL(1, bf.get_nMsg());
}

// Test case for BloomFilter::bloom_check
void test_bloomfilter_bloom_check(void) {
  BloomFilter bf(10, 5, 16, 50);
  unsigned char msg[] = "ABCDE";
  int msgSize = sizeof(msg) - 1;  // exclude null terminator
  TEST_ASSERT_EQUAL(0, bf.bloom_check(msg, msgSize));
  bf.bloom_add(msg, msgSize);
  TEST_ASSERT_EQUAL(1, bf.bloom_check(msg, msgSize));
}

// Dummy setUp and tearDown functions for Unity
void setUp(void) {}
void tearDown(void) {}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bloomfilter_constructor);
  RUN_TEST(test_bloomfilter_bloom_add);
  RUN_TEST(test_bloomfilter_bloom_check);
  UNITY_END();
  return 0;
}
