#include "LoRaPacket.h"
#include "Utils.h"
#include "CRC32.h"
#include <cstdint>
#include "Packet.h"
#include <vector>
#include <boost/crc.hpp>

#define DUCK_ERR_NONE 0
#define DUCK_ERR_SETUP -5100
#define ERR_SYNCWORD 46

using namespace std;


int LoraPacket::formLoRaPacket(vector<uint8_t> cdpData) {
 

  
  //isnerting preamble
  vector<uint8_t> preamble = {'D', 'U', 'C', 'K', 'C', 'A', 'L', 'L'};
  loraBuffer.insert(loraBuffer.end(), preamble.begin(), preamble.end());

  cout << "Preamble: " << duckutils::convertToHex(preamble.data(), preamble.size()).c_str() << endl;
  
  //inserting header
  
  header={3, 2};
  loraBuffer.insert(loraBuffer.end(), header.begin(), header.end());
  cout << "Header: "<< duckutils::convertToHex(header.data(), header.size()).c_str() << endl;

  //inserting header crc
  headerCRC = calculateCRCHeader(header);
  loraBuffer.insert(loraBuffer.end(), headerCRC.begin(), headerCRC.end());
  cout << "HeaderCRC: " << duckutils::convertToHex(headerCRC.data(), headerCRC.size()).c_str() << endl;

  //inserting payload
  payload = cdpData;
  loraBuffer.insert(loraBuffer.end(), payload.begin(), payload.end());
  cout <<"Payload: " << duckutils::convertToHex(payload.data(), payload.size()).c_str() << endl;

  //inserting payloadCRC
  payloadCRC = calculateCRCHeader(payload);
  loraBuffer.insert(loraBuffer.end(), payloadCRC.begin(), payloadCRC.end());

  cout << "Payload CRC: " << duckutils::convertToHex(payloadCRC.data(), payloadCRC.size()).c_str() << endl;

  // output lora packet
  cout << "Constructed Lora Packet: " << duckutils::convertToHex(loraBuffer.data(), loraBuffer.size()).c_str() << endl;
  return DUCK_ERR_NONE;
  }



int LoraPacket::setSyncWord(uint8_t syncWord) {
   if (syncWord == PRIVATE_CHANNEL){
   cout << "Setting channel to Private mode" << endl;
    return DUCK_ERR_NONE;
   }
   else if (syncWord == PUBLIC_CHANNEL){
    cout << "Setting channel to Public mode" << endl;
    return DUCK_ERR_NONE;
   }
   else {
    cout << "Error: Incorrect Syncword "<< endl;
    return ERR_SYNCWORD;
   }
    
}

vector<uint8_t> LoraPacket::calculateCRCHeader(vector<uint8_t> vec) {

    boost::crc_32_type crc_calculator;
    crc_calculator.reset();
    crc_calculator.process_bytes(vec.data(),vec.size());
    uint32_t crc= crc_calculator.checksum();
    vector<uint8_t> CRC = duckutils::convertNumToVector(crc);
    return CRC;

    
    
}




 