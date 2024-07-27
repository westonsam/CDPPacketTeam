#include "Packet.h"
#include "Utils.h"
#include "BloomFilter.h"
#include "CRC32.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>

using namespace std;

#define DUCK_ERR_NONE 0
#define DUCKPACKET_ERR_SIZE_INVALID -4000
#define DUCKPACKET_ERR_TOPIC_INVALID -4001
#define DUCKPACKET_ERR_MAX_HOPS -4002
#define DUCK_ERR_ID_TOO_LONG -5101
#define DUCK_ERR_OTA -5200

void Packet::getMessageId(BloomFilter *filter, uint8_t message_id[MUID_LENGTH])
{
  bool getNewUnique = true;
  while (getNewUnique)
  {
    duckutils::getRandomBytes(MUID_LENGTH, message_id);
    getNewUnique = filter->bloom_check(message_id, MUID_LENGTH);
    cout << "prepareForSending: new MUID -> " << duckutils::convertToHex(message_id, MUID_LENGTH).c_str() << endl;
  }
}

int Packet::prepareForSending(BloomFilter *filter, vector<uint8_t> dduid, uint8_t Topic, uint8_t duckType, uint8_t hopCount, vector<uint8_t> data)
{

  int data_length = data.size();

  this->reset();

  // Check data exists within correct size parameters
  if (data.empty())
  {
    cout << "Error: Duck Packet Empty";
    return DUCKPACKET_ERR_SIZE_INVALID;
  }
  else if (data_length > MAX_DATA_LENGTH)
  {
    cout << "Error: Duck Packet Too Large" << endl;
    return DUCKPACKET_ERR_SIZE_INVALID;
  }

  cout << "prepareForSending: DATA LENGTH: " << data_length << " - TOPIC (" << Packet::topicToString(topic).c_str() << ")" << endl;

  uint8_t message_id[MUID_LENGTH];
  getMessageId(filter, message_id);

  calculateCRC(data);

  // ----- insert packet header  -----

  // source device uid
  buffer.insert(buffer.end(), sduid.begin(), sduid.end());

  // destination device uid
  buffer.insert(buffer.end(), dduid.begin(), dduid.end());
  //this->dduid = dduid;
  duckutils::printVector(dduid, dduid.size());

  // message uid
  buffer.insert(buffer.end(), &message_id[0], &message_id[MUID_LENGTH]);
  //this->muid = muid;

  // topic
  
  buffer.push_back(Topic);
  //this->topic = topic;
  //cout << topic << endl;
  

  // duckType
  buffer.push_back(duckType);
  //this->duckType = duckType;

  // hop count
  buffer.push_back(hopCount);
  //this->hopCount = hopCount;

  // data crc
  vector<uint8_t> dataCRC = duckutils::convertNumToVector(dcrc);
  buffer.insert(buffer.end(), dataCRC.begin(), dataCRC.end());
  //this->dcrc = dcrc;
  
  // data
  buffer.insert(buffer.end(), data.begin(), data.end());

  // ----- print packet -----
  const string bufferString = duckutils::convertToHex(buffer.data(), buffer.size()).c_str();

  cout << "SDuid:        " << duckutils::convertToHex(sduid.data(), sduid.size()).c_str() << endl;
  cout << "DDuid:        " << duckutils::convertToHex(dduid.data(), dduid.size()).c_str() << endl;
  cout << "Muid:         " << bufferString.substr(MUID_POS * 2, (TOPIC_POS - MUID_POS) * 2) << endl;
  cout << "Topic:        " << bufferString.substr((TOPIC_POS) * 2, (DUCK_TYPE_POS - TOPIC_POS) * 2) << endl;
  cout << "Duck Type:    " << bufferString.substr((DUCK_TYPE_POS) * 2, (HOP_COUNT_POS - DUCK_TYPE_POS) * 2) << endl;
  cout << "Hop Count:    " << bufferString.substr((HOP_COUNT_POS) * 2, (DATA_CRC_POS - HOP_COUNT_POS) * 2) << endl;
  cout << "Data CRC:     " << bufferString.substr((DATA_CRC_POS) * 2, (DATA_POS - DATA_CRC_POS) * 2) << endl;
  cout << "Data:         " << bufferString.substr(DATA_POS * 2) << endl;
  cout << "Built packet: " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;
  return DUCK_ERR_NONE;
}

void Packet::setDuckId(vector<uint8_t> duckId)
{
  sduid = duckId;
}

void Packet::calculateCRC(vector<uint8_t> data)
{
  std::vector<uint8_t> encryptedData;
  uint32_t value;
  value = crc32<IEEE8023_CRC32_POLYNOMIAL>(0xFFFFFFFF, buffer.begin(), buffer.end());
  dcrc = value;
  
}



