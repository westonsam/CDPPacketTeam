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
#define DUCK_ERR_ID_TOO_LONG   -5101
#define DUCK_ERR_OTA           -5200


void Packet::getMessageId(BloomFilter *filter, uint8_t message_id[MUID_LENGTH])
{

  bool getNewUnique = true;
  while (getNewUnique)
  {
    duckutils::getRandomBytes(MUID_LENGTH, message_id);
    getNewUnique = filter->bloom_check(message_id, MUID_LENGTH);
    cout << "[I]" << "prepareForSending: new MUID -> " << duckutils::convertToHex(message_id, MUID_LENGTH).c_str() << endl;
  }
}

int Packet::prepareForSending(BloomFilter *filter, vector<uint8_t> destinationId, uint8_t duckType, uint8_t topic, vector<uint8_t> data)
{

  int data_length = data.size();

  this->reset();

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
  cout << "SDuid:     " << duckutils::convertToHex(sduid.data(), sduid.size()).c_str() << endl;

  // destination device uid
  buffer.insert(buffer.end(), dduid.begin(), dduid.end());
  cout << "DDuid:     " << duckutils::convertToHex(destinationId.data(), destinationId.size()).c_str() << endl;

  // message uid
  buffer.insert(buffer.end(), &message_id[0], &message_id[MUID_LENGTH]);
  cout << "Muid:      " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;

  // topic
  buffer.insert(buffer.end(), topic);
  cout << "Topic:     " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;

  // duckType
  buffer.insert(buffer.end(), duckType);
  cout << "Duck Type: " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;

  // hop count
  buffer.insert(buffer.end(), 0x00);
  cout << "Hop Count: " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;

  // data crc
  buffer.insert(buffer.end(), dcrc);
  cout << "Data CRC:  " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;

  // ----- insert data -----

  buffer.insert(buffer.end(), data.begin(), data.end());
  cout << "Data:      " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;

  cout << "Built packet: " << duckutils::convertToHex(buffer.data(), buffer.size()).c_str() << endl;
  return DUCK_ERR_NONE;
}

void Packet::calculateCRC(vector<uint8_t> data)
{
  std::vector<uint8_t> encryptedData;
  uint32_t value;
  value = crc32<IEEE8023_CRC32_POLYNOMIAL>(0xFFFFFFFF, buffer.begin(), buffer.end());
  dcrc = value;
}