#include "BloomFilter.h"
#include <iostream>
using std::cout;
using std::endl;

void BloomFilter::set_hash_results(unsigned char *msg, int msgSize,
                                   std::unique_ptr<unsigned int[]> &hashResults)
{
    for (int i = 0; i < this->numHashes; i++)
    {
        unsigned int hashResult = djb2Hash(msg, this->Seeds[i], msgSize);
        unsigned int totalBitSize = this->numSectors * this->bitsPerSector;
        hashResult = hashResult % totalBitSize;

        int hashCollision;
        do
        {
            hashCollision = 0;
            for (int j = 0; j < i; j++)
            {
                if (hashResult == hashResults[j])
                {
                    hashCollision = 1;
                }
            }
            if (hashCollision == 1)
            {
                hashResult = (hashResult + 1) % totalBitSize;
            }
        } while (hashCollision == 1);

        hashResults[i] = hashResult;
    }
}

int BloomFilter::bloom_check(unsigned char *msg, int msgSize)
{

    std::unique_ptr<unsigned int[]> hashResults(new unsigned int[this->numHashes]);
    set_hash_results(msg, msgSize, hashResults);

    std::unique_ptr<int[]> sectors(new int[this->numHashes]);
    std::unique_ptr<unsigned int[]> slots(new unsigned int[this->numHashes]);
    set_sectors_and_slots(hashResults, sectors, slots);

    if (is_collision(filter1, sectors, slots))
    {
        return 1;
    }
    if (is_collision(filter2, sectors, slots))
    {
        return 1;
    }
    return 0;
}

int BloomFilter::is_collision(const unsigned int *filter,
                              const std::unique_ptr<int[]> &sectors,
                              const std::unique_ptr<unsigned int[]> &slots)
{
    for (int i = 0; i < this->numHashes; i++)
    {
        if ((filter[sectors[i]] & slots[i]) == 0)
        { //
            return 0;
        }
    }
    return 1;
}

void BloomFilter::set_sectors_and_slots(const std::unique_ptr<unsigned int[]> &hashResults,
                                        std::unique_ptr<int[]> &sectors, std::unique_ptr<unsigned int[]> &slots)
{
    for (int i = 0; i < this->numHashes; i++)
    {
        sectors[i] = hashResults[i] / this->bitsPerSector;
        int offset = hashResults[i] % this->bitsPerSector;

        unsigned int x = pow(2, (this->bitsPerSector) - 1);
        slots[i] = x >> offset;
    }
}

void BloomFilter::bloom_add(unsigned char *msg, int msgSize)
{

    this->nMsg += 1;

    std::unique_ptr<unsigned int[]> hashResults(new unsigned int[this->numHashes]);
    set_hash_results(msg, msgSize, hashResults);

    std::unique_ptr<int[]> sectors(new int[this->numHashes]);
    std::unique_ptr<unsigned int[]> slots(new unsigned int[this->numHashes]);
    set_sectors_and_slots(hashResults, sectors, slots);

    for (int i = 0; i < this->numHashes; i++)
    {
        if (this->activeFilter == 1)
        {
            this->filter1[sectors[i]] = this->filter1[sectors[i]] | slots[i];
        }
        else
        {
            this->filter2[sectors[i]] = this->filter2[sectors[i]] | slots[i];
        }
    }

    if (this->nMsg >= this->maxMsgs)
    {
        if (this->activeFilter == 1)
        {
            cout << "Freezing filter 1, switching to filter 2" << endl;
            for (int i = 0; i < (this->numSectors) / (this->bitsPerSector); i++)
            {
                this->filter2[i] = 0;
            }
            this->activeFilter = 2;
        }
        else
        {
            cout << "Freezing filter 2, switching to filter 1" << endl;
            for (int i = 0; i < (this->numSectors) / (this->bitsPerSector); i++)
            {
                this->filter1[i] = 0;
            }
            this->activeFilter = 1;
        }
        this->nMsg = 0;
    }
}

unsigned int BloomFilter::djb2Hash(unsigned char *str, int seed, int msgSize)
{ // "version" of djb2Hash with seed
    unsigned int hash = seed;
    int c;

    // TODO: test that for loop outputs the same value as while loop
    //   while ((c = *str++))
    //       hash = ((hash << 5) + hash) + c; // (hash * 33) + c

    for (int i = 0; i < msgSize; i++)
    {
        c = *str++;
        hash = ((hash << 5) + hash) + c; // (hash * 33) + c
    }
    return hash;
}
