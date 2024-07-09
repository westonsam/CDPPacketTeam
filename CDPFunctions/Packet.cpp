#include "Packet.h"
#include "Utils.h"
#include "bloomfilter.h"
#include <string>
using std::string;
using std::cout;
using std::endl;

void Packet::getMessageId(BloomFilter * filter, uint8_t message_id[MUID_LENGTH]) {

  bool getNewUnique = true;
  while (getNewUnique) {
    duckutils::getRandomBytes(MUID_LENGTH, message_id);
    getNewUnique = filter->bloom_check(message_id, MUID_LENGTH);
    cout << "[I]" << "prepareForSending: new MUID -> " << duckutils::convertToHex(message_id, MUID_LENGTH).c_str() << endl;
  }
}
