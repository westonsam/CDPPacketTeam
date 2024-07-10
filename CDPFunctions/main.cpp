#include <iostream>
#include <fstream> //for file handling
#include <cstdlib>   // for exit()
#include <string>
#include <cstdint> //used for uint8_t
#include <vector> //includes vectors, similar arrays
#include <stdexcept>
#include "Packet.h"
#include "BloomFilter.h"
#include "Utils.h"
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

vector<uint8_t> convertStringToVector (string line);
string convertVectorToString (vector<uint8_t> buffer);
string readFile(string filename);
void writeFile(string filename, vector<uint8_t> RxData);
vector<string> processInputFile(string input);

string readFile(string filename) {
    ifstream fin;
    string data = "";
    fin.open(filename);
    if(!fin.is_open()) {
        cout << "Input file failed to open.\n";
        return "Fail";
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(fin, line)) {
        data += line;
    }
    fin.close();
    return data;
}

void writeFile(string filename, vector<uint8_t> TxData) {
    ofstream fout;

    fout.open(filename);
      if(!fout.is_open()) {
        cout << "Output file failed to open." << endl;
        exit(EXIT_FAILURE);
      }
    string data;   
    data = convertVectorToString(TxData);
    fout << data << endl;
    fout.close();
}

vector<uint8_t> convertStringToVector (string line) {
    
    vector<uint8_t> buffer;
    for (char str_char : line)
        buffer.push_back(uint8_t(str_char));
    return buffer;
}


string convertVectorToString (vector<uint8_t> buffer) {
    string str;
    for (int i = 0; i < buffer.size(); i++) {
        str += buffer[i];
    }
    return str;
}

vector<string> processInputFile(string input){
    string DDUID, TOPIC, DATA;
    DDUID = input.substr(input.find("DUID:"), input.find("TOPIC:"));
    TOPIC = input.substr(input.find("TOPIC:"), input.find("DATA:"));
    DATA = input.substr(input.find("DATA:"), input.length() - 1);
    vector<string> processed = {DDUID, TOPIC, DATA};
    cout <<  "PROCESSED:" << endl;
    cout << processed[0] << endl;
    cout << processed[1] << endl;
    cout << processed[2] << endl;
    return processed;
}


int main() {
    string inputData = readFile("infile.txt");
    cout << inputData << endl;
    processInputFile(inputData);
    vector<string> processed = processInputFile(inputData);
    vector<uint8_t> dduid = convertStringToVector(processed[0]);
    uint8_t topic = Packet::stringToTopic(processed[1]);

    vector<uint8_t> data = convertStringToVector(processed[2]);
    Packet dp;
    BloomFilter filter = BloomFilter(DEFAULT_NUM_SECTORS, DEFAULT_NUM_HASH_FUNCS, DEFAULT_BITS_PER_SECTOR, DEFAULT_MAX_MESSAGES);
    dp.prepareForSending(&filter, dduid, DuckType::MAMA,topic, data);
    cout << convertVectorToString(dp.getBuffer());
}