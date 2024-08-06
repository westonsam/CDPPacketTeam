#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include "Packet.h"
//#include "BloomFilter.h"
#include "Utils.h"

using namespace std;

string readFile(string filename);
void writeFile(string filename, vector<uint8_t> RxData);
vector<string> processInputFile(string input);


string readFile(const string filename)
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

void writeFileWebServer (string filename, vector<string> TxData) {
    ofstream fout;
    fout.open(filename);
    if (!fout.is_open())
    {
        cout << "Output file failed to open." << endl;
        exit(EXIT_FAILURE);
    }
    fout<< "DDUID:" << TxData[0] << endl;
    fout << "Topic:" << TxData[1] << endl;
    fout<< "Data:" << TxData[2] <<endl;
    fout <<"Date:" << TxData[3] << endl;
    fout << "Time:" << TxData[4] << endl;
    fout.close();
}

vector<string> processInputFile(string input)
{
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


int main()
{

    // Instantiate Packet Object
    Packet dp;

    vector<uint8_t> packetRecieved(24);
    packetRecieved = {'P', 'A', 'P', 'A' , 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A' , 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A' , 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A'};

    dp.setBuffer(packetRecieved);

    //gets payload generated
    vector<uint8_t> payload = dp.getBuffer();


   //parses sections of received cdp payload
    vector<string> outputFileData;
    outputFileData = dp.decodePacket(payload);

    string CDP = duckutils::convertVectorToString(payload);

    cout << "CDP packet as a string: " << CDP << endl;

    writeFileWebServer("outfile.txt", outputFileData);



    return 0;
}