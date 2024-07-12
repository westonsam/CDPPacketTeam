#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <vector> 

using std::cout;
using std::endl;


const int DEFAULT_NUM_SECTORS = 312;
const int DEFAULT_NUM_HASH_FUNCS = 2;
const int DEFAULT_BITS_PER_SECTOR = 32;
const int DEFAULT_MAX_MESSAGES = 100;

// two-phase bloom filter
class BloomFilter
{
public:
  /**
   * @brief Initialize a bloom filter
   *
   * @param numSectors, The number of sectors in filter
   * @param numHashes, The number of hash functions
   * @param bitsPerSector, The size of a sector in bits
   * @param maxMsgs, The maximum number of messages until the next filter is used.
   */
  BloomFilter(int numSectors, int numHashes, int bitsPerSector, int maxMsgs)
  {
    this->numSectors = numSectors;
    this->numHashes = numHashes;
    this->bitsPerSector = bitsPerSector;
    this->activeFilter = 1;
    this->maxMsgs = maxMsgs;
    this->nMsg = 0;
    cout << "BloomFilter constructor: numSectors: " << numSectors << ", numHashes: " << numHashes << ", bitsPerSector: " << bitsPerSector << ", maxMsgs: " << maxMsgs << endl;

    cout << "initialize bloom filter 1";
    // Initialize the bloom filters, fill with 0's
    this->filter1 = new unsigned int[this->numSectors];
    cout << "Filter 1 address" << this->filter1 << endl;
    if (this->filter1 == NULL)
    {
      cout << "Memory allocation for Bloom Filter 1 failed!" << endl;
      exit(0);
    }
    for (int n = 0; n < this->numSectors; n++)
    {
      this->filter1[n] = 0;
    }
    cout << "Initialized BF1, " << numSectors << " slots, " << this->numSectors << " this->numSectors" << endl;

    cout << "initialize bloom filter 2";
    this->filter2 = new unsigned int[this->numSectors];
    cout << "Filter 2 address " << this->filter2 << endl;
    if (this->filter2 == NULL)
    {
      cout << "Memory allocation for Bloom Filter 2 failed!" << endl;
      exit(0);
    }
    for (int n = 0; n < this->numSectors; n++)
    {
      this->filter2[n] = 0;
    }
    cout << "Initialized BF2, " << numSectors << " slots, " << this->numSectors << " this->numSectors" << endl;

    cout << "initialize random seeds";
    // get random seeds for hash functions
    int *Seeds = new int[numHashes];
    if (Seeds == NULL)
    {
      cout << "Memory allocation for seeds failed!" << endl;
      exit(0);
    }
    srand(time(NULL));
    bool seedCollision = true;
    bool collision;
    while (seedCollision)
    {
      collision = false;
      for (int i = 0; i < numHashes; i++)
      {
        int r = rand();
        if (Seeds[i] == r)
        {
          r = rand();
          collision = true;
          break;
        }
        Seeds[i] = r;
      }
      if (!collision)
      {
        seedCollision = false;
      }
    }

    for (int i = 0; i < numHashes; i++)
    {
      cout << "random seed " << i + 1 << ": " << Seeds[i] << endl;
    }
    this->Seeds = Seeds;

    cout << "bloom_init end" << endl;
  }
  

  /**
   * @return 1 if we (possibly) found word; for a new word returns 0
   */
  int bloom_check(unsigned char *msg, int msgSize);

  void bloom_add(unsigned char *msg, int msgSize);

  int get_numSectors()
  {
    return numSectors;
  }

  int get_numHashes()
  {
    return numHashes;
  }

  int get_bitsPerSector()
  {
    return bitsPerSector;
  }

  int get_maxMsgs()
  {
    return maxMsgs;
  }

  int get_nMsg()
  {
    return nMsg;
  }

private:
  unsigned int *filter1;
  unsigned int *filter2;
  int activeFilter; // 1 or 2
  int numSectors;   // power of 2
  int numHashes;
  int bitsPerSector; // power of 2, smaller than numSectors
  int nMsg;
  int maxMsgs;
  int *Seeds;

  static unsigned int djb2Hash(unsigned char *str, int seed, int msgSize);

  /**
   * Generates a unique unsigned int index for each hash
   *
   * @param msg, The message being hashed and converted into an index
   * @param msgSize, The size of msg
   * @param hashResults, Output as the bit index within the filter
   */
  void set_hash_results(unsigned char *msg, int msgSize,
                        std::unique_ptr<unsigned int[]> &hashResults);

  void set_sectors_and_slots(const std::unique_ptr<unsigned int[]> &hashResults,
                             std::unique_ptr<int[]> &sectors, std::unique_ptr<unsigned int[]> &slots);

  int is_collision(const unsigned int *filter,
                   const std::unique_ptr<int[]> &sectors,
                   const std::unique_ptr<unsigned int[]> &slots);
};

#endif