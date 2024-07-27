#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include "LoRaPacket.h"
#include "Packet.h"
#include "BloomFilter.h"
#include "Utils.h"

using namespace std;

vector<uint8_t> convertStringToVector(string line);
string convertVectorToString(vector<uint8_t> buffer);
string readFile(string filename);
void writeFile(string filename, vector<uint8_t> RxData);
vector<string> processInputFile(string input);
vector<uint8_t> reconcatenate (uint8_t startPosition, uint8_t endPosition,vector<uint8_t> oldVec);

string readFile(string filename)
{
    ifstream fin;
    string data = "";
    fin.open(filename);
    if (!fin.is_open())
    {
        cout << "Input file failed to open.\n";
        return "Fail";
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(fin, line))
    {
        data += line;
    }
    fin.close();
    return data;
}

void writeFile(string filename, string TxData)
{
    ofstream fout;

    fout.open(filename);
    if (!fout.is_open())
    {
        cout << "Output file failed to open." << endl;
        exit(EXIT_FAILURE);
    }
    fout << TxData << endl;
    fout.close();
}

vector<uint8_t> convertStringToVector(string line)
{

    vector<uint8_t> buffer;
    for (char str_char : line)
        buffer.push_back(uint8_t(str_char));
    return buffer;
}

string convertVectorToString(vector<uint8_t> buffer)
{
    string str;
    for (int i = 0; i < buffer.size(); i++)
    {
        str += buffer.at(i);
    }
    return str;
}

vector<string> processInputFile(string input)
{
    /*size_t duidStart = input.find("DUID:");
    size_t topicStart = input.find("TOPIC:");
    size_t dataStart = input.find("DATA:");
    size_t dateStart = input.find("DATE:");

    string DDUID = input.substr(duidStart + 5, topicStart - duidStart - 5);
    string TOPIC = input.substr(topicStart + 6, dataStart - topicStart - 6);
    string DATA = input.substr(dataStart + 5, dateStart - dataStart - 5);

    vector<string> processed = {DDUID, TOPIC, DATA};*/


    // Split input data by section
    string DDUID, TOPIC, DATA;
    DDUID = input.substr(input.find("DUID:"), input.find("TOPIC:")).substr(5);
    TOPIC = input.substr(input.find("TOPIC:"), input.find("DATA:") - input.find("TOPIC:")).substr(6);
    DATA = input.substr(input.find("DATA:"), input.find("DATE:") - input.find("DATA:")).substr(5);
    vector<string> processed = {DDUID, TOPIC, DATA};

    // Print Processed Data
    cout << "PROCESSED INPUT FILE:" << endl;
    cout << processed[0] << endl;
    cout << processed[1] << endl;
    cout << processed[2] << endl;
    cout << endl;
    return processed;
}

int decodePacket(vector<uint8_t> cdpPayload){

    auto start = std::chrono::system_clock::now();
    
   
  vector<uint8_t> sduid;
  sduid = reconcatenate(SDUID_POS, DDUID_POS, cdpPayload);
  
  vector<uint8_t> dduid;
  dduid = reconcatenate(DDUID_POS, MUID_POS, cdpPayload);
  
  vector<uint8_t> muid;
  muid = reconcatenate(MUID_POS, TOPIC_POS, cdpPayload);
  
  vector<uint8_t> topic;
  topic = reconcatenate(TOPIC_POS, DUCK_TYPE_POS+1, cdpPayload);
  //topic.push_back(cdpPayload.at(TOPIC_POS));
  //topic.insert(topic.end(), cdpPayload.at(TOPIC_POS));
  
  duckutils::printVector(topic, topic.size());

  //uint8_t Topic = cdpPayload.at(TOPIC_POS);
  
  vector<uint8_t> duckType;
  duckType = reconcatenate(DUCK_TYPE_POS, HOP_COUNT_POS, cdpPayload);
  
  vector<uint8_t> hopCount;
  hopCount = reconcatenate(HOP_COUNT_POS, DATA_CRC_POS, cdpPayload);
  
  vector<uint8_t> dcrc;
  dcrc = reconcatenate(DATA_CRC_POS, DATA_POS, cdpPayload);
  
  vector<uint8_t> data;
  data = reconcatenate(DATA_POS, cdpPayload.size(), cdpPayload);
  
  
    
  cout << "Recieved sduid: " << duckutils::convertToHex(sduid.data(), sduid.size()).c_str() << endl;
  cout << "Recieved dduid: " <<duckutils::convertToHex(dduid.data(), dduid.size()).c_str() << endl;
  cout << "Recieved muid: " <<duckutils::convertToHex(muid.data(), muid.size()).c_str() << endl;
  cout << "Recieved topic: " << duckutils::convertToHex(topic.data(), topic.size()).c_str() << endl;
  cout << "Recieved duckType: " << duckutils::convertToHex(duckType.data(), duckType.size()).c_str() << endl;
  cout << "Recieved hopCount: " << duckutils::convertToHex(hopCount.data(), hopCount.size()).c_str() << endl;
  cout << "Recieved data CRC: " <<duckutils::convertToHex(dcrc.data(), dcrc.size()).c_str() << endl;
  cout << "Recieved data: " <<duckutils::convertToHex(data.data(), data.size()).c_str() << endl;

 auto end = std::chrono::system_clock::now();
 time_t end_time = std::chrono::system_clock::to_time_t(end);
 cout << "finished computation at " << ctime(&end_time);

ofstream fout;
fout.open("outfile2.txt");
    if (!fout.is_open())
    {
        cout << "Output file failed to open." << endl;
        exit(EXIT_FAILURE);
    }
string dduid_str = convertVectorToString(dduid);
string topic_str = convertVectorToString(topic);

string data_str = convertVectorToString(data);

fout << "DUID:" << dduid_str << endl;
    fout << "TOPIC:" << topic_str << endl;
    //fout << "TOPIC:" << Topic << endl;
    fout << "DATA:" << data_str << endl;

    fout.close();

  /*vector<uint8_t> outputFileData;
  outputFileData.insert(outputFileData.end(), dduid.begin(), dduid.end());
  outputFileData.insert(outputFileData.end(), topic.begin(), topic.end());
  outputFileData.insert(outputFileData.end(), data.begin(), data.end());*/

  return 0;

}

vector<uint8_t> reconcatenate (uint8_t startPosition, uint8_t endPosition,vector<uint8_t> oldVec){
    vector<uint8_t> newVec;
    for (uint8_t i = startPosition; i < endPosition; i++) {
        newVec.push_back(oldVec.at(i));
    }
    return newVec;
}

int main()
{
    // Read from input file
    string inputData = readFile("infile.txt");
    vector<string> processed = processInputFile(inputData);
    vector<uint8_t> dduid = convertStringToVector(processed[0]);
    uint8_t topic = Packet::stringToTopic(processed[1]);
    vector<uint8_t> data = convertStringToVector(processed[2]);

    // Instantiate Packet Object
    Packet dp;
    // Set Duck ID
    dp.setDuckId(convertStringToVector("PAPA0001"));

    // Create BloomFilter
    BloomFilter filter = BloomFilter(DEFAULT_NUM_SECTORS, DEFAULT_NUM_HASH_FUNCS, DEFAULT_BITS_PER_SECTOR, DEFAULT_MAX_MESSAGES);

    // Create formatted CDP packet in the buffer property of dp object
    dp.prepareForSending(&filter, dduid, topic, DuckType::MAMA, 0x00, data);

    vector<uint8_t> payload = dp.getBuffer();
    int l = decodePacket(payload);

    // Write formatted packet to output file
    string cdpPacket = duckutils::convertToHex(dp.getBuffer().data(), dp.getBuffer().size()).c_str();

    
    cout << "(SHOULD MATCH OUTFILE.TXT)" << endl;
    writeFile("outfile.txt", cdpPacket);

    //LoraPacket lp;
    //lp.formLoRaPacket(dp.getBuffer());
}