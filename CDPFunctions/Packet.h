#include <iostream>
#include <fstream> //for file handling
#include <cstdlib>   // for exit()
#include <string>
#include <cstdint> //used for uint8_t
#include <vector> //includes vectors, similar arrays
#include "bloomfilter.h"

#define MAX_HOPS 6

// field/section length (in bytes)
#define PACKET_LENGTH 256
#define DUID_LENGTH 8
#define MUID_LENGTH 4
#define DATA_CRC_LENGTH 4
#define HEADER_LENGTH 27

// field/section offsets
#define SDUID_POS 0
#define DDUID_POS 8
#define MUID_POS 16
#define TOPIC_POS 20
//#define PATH_OFFSET_POS 21
#define DUCK_TYPE_POS 21
#define HOP_COUNT_POS 22
#define DATA_CRC_POS 23
#define DATA_POS HEADER_LENGTH // Data section starts immediately after header

#define RESERVED_LENGTH 2
//#define MAX_PATH_LENGTH (MAX_HOPS * DUID_LENGTH)
#define MAX_DATA_LENGTH (PACKET_LENGTH - HEADER_LENGTH)
//#define MAX_PATH_OFFSET (PACKET_LENGTH - DUID_LENGTH - 1)

using std::string;
using std::vector;

/**
 * @brief Use this DUID to send to all PapaDucks
 * 
 */
static std::vector<uint8_t> ZERO_DUID = {0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00};

/**
 * @brief Use this DUID to be received by every duck in the network
 * 
 */
static std::vector<uint8_t> BROADCAST_DUID = {0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFF, 0xFF};

/**
 * @brief Use this DUID to be received by every duck in the network
 * 
 */
static std::vector<uint8_t> PAPADUCK_DUID = {0x50, 0x61, 0x70, 0x61,
                                           0x44, 0x75, 0x63, 0x6B};


class Packet {
    public:

        Packet() {}
        Packet(vector<uint8_t> duckId) {this->SDUID = duckId;}
        void setDuckId(string duckId);
        vector<uint8_t> getDuckId() { return this->SDUID = SDUID;}
        int prepareForSending(BloomFilter *filter, const vector<uint8_t> destinationId, uint8_t duckType, uint8_t topic, vector<uint8_t> data[MAX_DATA_LENGTH]);
        // prepareForRelaying
        vector<uint8_t> getBuffer() {return buffer;}
        uint8_t getTopic() {return buffer[TOPIC_POS];}
        void reset() {vector<uint8_t>().swap(buffer);}
    private:
        vector<uint8_t> SDUID;
        // string DDUID;
        // string MUID;
        // string TOPIC;
        // string DUCKTYPE;
        // string HOPCOUNT;
        // string DCRC;
        // string DATA;
        vector<uint8_t> buffer;
        static void getMessageId(BloomFilter * filter, uint8_t message_id[MUID_LENGTH]);
};