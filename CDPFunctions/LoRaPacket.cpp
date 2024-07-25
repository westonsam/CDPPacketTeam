#include "LoRaPacket.h"
//#include <algorithim>
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
  vector<uint8_t> preamble = {10, 10, 10, 10, 10, 10, 10, 10};
  loraBuffer.insert(loraBuffer.end(), preamble.begin(), preamble.end());
  cout << "Preamble: " << duckutils::convertToHex(preamble.data(), preamble.size()).c_str() << endl;
  uint8_t preambleSize = duckutils::countNumBytes(preamble);
  cout << "Size of preamble: " << preambleSize << " bytes " << endl;
  
  //inserting header
  uint8_t payloadSize = duckutils::countNumBytes(cdpData);
  //1st byte: payload size, 
  //2nd byte: [0:2] FEC CR, [3] CRC prescence, [4:7] headerCRC << 4, 
  //3rd byte: [0:3] headerCRC >> 4, [4:7] 0000
  header = {payloadSize, 0, 0};
  loraBuffer.insert(loraBuffer.end(), header.begin(), header.end());
  cout << "Header: "<< duckutils::convertToHex(header.data(), header.size()).c_str() << endl;
  cout << "Size of header: " << duckutils::countNumBytes(header) << " bytes " << endl;

  /*//inserting header crc
  headerCRC = calculateCRCHeader(header);
  loraBuffer.insert(loraBuffer.end(), headerCRC.begin(), headerCRC.end());
  cout << "HeaderCRC: " << duckutils::convertToHex(headerCRC.data(), headerCRC.size()).c_str() << endl;
  cout << "Size of headerCRC: " << duckutils::countNumBytes(headerCRC) << " bytes " << endl;*/

  //inserting payload
  payload = cdpData;
  loraBuffer.insert(loraBuffer.end(), payload.begin(), payload.end());
  cout <<"Payload: " << duckutils::convertToHex(payload.data(), payload.size()).c_str() << endl;
  cout << "Size of payload: " << duckutils::countNumBytes(payload) << " bytes " << endl;

  //inserting payloadCRC
  payloadCRC = calculateCRCHeader(payload);
  loraBuffer.insert(loraBuffer.end(), payloadCRC.begin(), payloadCRC.end());
  cout << "Payload CRC: " << duckutils::convertToHex(payloadCRC.data(), payloadCRC.size()).c_str() << endl;
  cout << "Size of payloadCRC: " << duckutils::countNumBytes(payloadCRC) << " bytes " << endl;

  //check size of lora packet
  cout << "Size of LoRa packet: " << duckutils::countNumBytes(loraBuffer) << " bytes " << endl;

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




 