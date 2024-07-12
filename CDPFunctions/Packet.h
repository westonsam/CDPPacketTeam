#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <fstream> //for file handling
#include <cstdlib> // for exit()
#include <string>
#include <cstdint> //used for uint8_t
#include <vector>  //includes vectors, similar arrays
#include "BloomFilter.h"
#include "Utils.h"

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
// #define PATH_OFFSET_POS 21
#define DUCK_TYPE_POS 21
#define HOP_COUNT_POS 22
#define DATA_CRC_POS 23
#define DATA_POS HEADER_LENGTH // Data section starts immediately after header

#define RESERVED_LENGTH 2
// #define MAX_PATH_LENGTH (MAX_HOPS * DUID_LENGTH)
#define MAX_DATA_LENGTH (PACKET_LENGTH - HEADER_LENGTH)
// #define MAX_PATH_OFFSET (PACKET_LENGTH - DUID_LENGTH - 1)

enum DuckType {
  /// A Duck of unknown type
  UNKNOWN = 0x00,
  /// A PapaDuck
  PAPA = 0x01,
  /// A MamaDuck
  MAMA = 0x02,
  /// A DuckLink
  LINK = 0x03,
  /// A Detector Duck
  DETECTOR = 0x04,
  MAX_TYPE
};

enum topics
{
    /// generic message (e.g non emergency messages)
    status = 0x10,
    /// captive portal message
    cpm = 0x11,
    /// a gps or geo location (e.g longitude/latitude)
    location = 0x12,
    /// sensor captured data
    sensor = 0x13,
    /// an allert message that should be given immediate attention
    alert = 0x14,
    /// Device health status
    health = 0x15,
    // Send duck commands
    dcmd = 0x16,
    // MQ7 Gas Sensor
    mq7 = 0xEF,
    // GP2Y Dust Sensor
    gp2y = 0xFA,
    // bmp280
    bmp280 = 0xFB,
    // DHT11 sensor
    dht11 = 0xFC,
    // ir sensor
    pir = 0xFD,
    // bmp180
    bmp180 = 0xFE,
    // Max supported topics
    max_topics = 0xFF
};

enum reservedTopic
{
    unused = 0x00,
    ping = 0x01,
    pong = 0x02,
    gps = 0x03,
    ack = 0x04,
    cmd = 0x05,
    max_reserved = 0x0F
};

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

class Packet
{
public:
    Packet() {} 
    void setDuckId(string duckId);
    vector<uint8_t> getDuckId() { return this->sduid = sduid; }
    int prepareForSending(BloomFilter *filter, const vector<uint8_t> destinationId, uint8_t duckType, uint8_t topic, vector<uint8_t> data);
    // prepareForRelaying
    std::vector<uint8_t> getBuffer() { return buffer; }
    uint8_t getTopic() { return buffer[TOPIC_POS]; }
    void reset() { vector<uint8_t>().swap(buffer); }
    /// Source Device UID (8 bytes)
    std::vector<u_int8_t> sduid;
    /// Destination Device UID (8 bytes)
    std::vector<uint8_t> dduid;
    /// Message UID (4 bytes)
    std::vector<uint8_t> muid;
    /// Message topic (1 byte)
    uint8_t topic;
    /// Offset to the Path section (1 byte)
    uint8_t path_offset;
    /// Type of ducks as define in DuckTypes.h
    uint8_t duckType;
    /// Number of times a packet was relayed in the mesh
    uint8_t hopCount;
    /// crc32 for the data section
    std::uint32_t dcrc;
    /// Data section
    std::vector<uint8_t> data;
    /// Path section (48 bytes max)
    std::vector<uint8_t> path;
    // time received
    unsigned long timeReceived;

    Packet(const std::vector<uint8_t> &buffer)
    {
        int buffer_length = buffer.size();
        sduid.assign(&buffer[SDUID_POS], &buffer[DDUID_POS]);
        dduid.assign(&buffer[DDUID_POS], &buffer[MUID_POS]);
        muid.assign(&buffer[MUID_POS], &buffer[TOPIC_POS]);
        topic = buffer[TOPIC_POS];
        duckType = buffer[DUCK_TYPE_POS];
        hopCount = buffer[HOP_COUNT_POS];
        dcrc = duckutils::toUint32(&buffer[DATA_CRC_POS]);
        data.assign(&buffer[DATA_POS], &buffer[buffer_length]);
    }
    static std::string topicToString(int topic)
    {
        switch (topic)
        {
        case topics::status:
            return "status";
        case topics::cpm:
            return "cpm";
        case topics::location:
            return "location";
        case topics::sensor:
            return "sensor";
        case topics::alert:
            return "alert";
        case topics::health:
            return "health";
        case topics::dcmd:
            return "dcmd";
        case topics::mq7:
            return "mq7";
        case topics::gp2y:
            return "gp2y";
        case topics::bmp280:
            return "bmp280";
        case topics::dht11:
            return "dht11";
        case topics::pir:
            return "pir";
        case topics::bmp180:
            return "bmp180";
        default:
            return "unknown";
        }
    }
    static int stringToTopic(string topic)
    {
        if (topic == "status") {
            return topics::status;
        } else if (topic == "cpm") {
            return topics::cpm;
        } else if (topic == "location") {
            return topics::location;
        } else if (topic == "sensor") {
            return topics::sensor;
        } else if (topic == "alert") {
            return topics::alert;
        } else if (topic == "health") {
            return topics::health;
        } else if (topic == "dcmd") {
            return topics::dcmd;
        } else if (topic == "mq7") {
            return topics::mq7;
        } else if (topic == "gp2y") {
            return topics::gp2y;
        } else if (topic == "bmp280") {
            return topics::bmp280;
        } else if (topic == "dht11") {
            return topics::dht11;
        } else if (topic == "pir") {
            return topics::pir;
        } else if (topic == "bmp180") {
            return topics::bmp180;
        } 
            return topics::status;
    }

private:
    vector<uint8_t> buffer;
    static void getMessageId(BloomFilter *filter, uint8_t message_id[MUID_LENGTH]);
    void calculateCRC(vector<uint8_t> data);
};

#endif // PACKET_H