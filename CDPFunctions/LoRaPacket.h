#ifndef LoRaPacket_H
#define LoRaPacket_H
#endif

#include <cstdint>
#include "Packet.h"
#include <vector>
#include "Utils.h"
#include <boost/crc.hpp>//CRC library

#define PRIVATE_CHANNEL 18 //0x12
#define PUBLIC_CHANNEL 52 //0x34

#define PREAMBLE_POS 0
#define HEADER_POS 12
#define HEADERCRC_POS 14
#define PAYLOAD_POS 16
#define PAYLOADCRC_POS 245
#define MAX_LORAPACKET_LENGTH 256


//typedef uint8_t byte;
using namespace std;

class LoraPacket {
    public: 
    friend class Packet;
    LoraPacket() {};

    /////////////////assuming LoRa packet is in explicit mode with coding rate of 4/8 for header///////////////
    //first 8 characters of preamble. last 4 will be randomly generated
    vector<uint8_t> partPreamble;  

    //characters used to sync receiver and transmitter
    vector<uint8_t> preamble;       

    //payload length tbd
    vector<uint8_t> header; 

    //CRC for header. length tbd 
    vector<uint8_t> headerCRC;

    //CDP packet. max size of 229 bytes
    vector<uint8_t> payload;

    //crc for CDP. length tbd
    vector<uint8_t> payloadCRC;


    LoraPacket(vector<uint8_t>& lorabuffer) {
        preamble.assign(&lorabuffer[PREAMBLE_POS], &lorabuffer[HEADER_POS]);
        header.assign(&lorabuffer[HEADER_POS], &lorabuffer[HEADERCRC_POS]);
        headerCRC.assign(&lorabuffer[HEADERCRC_POS], &lorabuffer[PAYLOAD_POS]);
        payload.assign(&lorabuffer[PAYLOAD_POS], &lorabuffer[PAYLOADCRC_POS]);
        payloadCRC.assign(&lorabuffer[PAYLOADCRC_POS], &lorabuffer[MAX_LORAPACKET_LENGTH]);
    }

    int formLoRaPacket(vector<uint8_t> cdpData);
    vector<uint8_t> calculateCRCHeader(vector<uint8_t> header);
    
    //int calculateCheckSumPayload(vector<byte> payload, vector<byte> payloadcrc);

    private:
    int setSyncWord(uint8_t syncWord); //set sync word used to communicate between radios. 0x12 for private and 0x34 for public channels.
    vector<uint8_t> loraBuffer;

    
};
