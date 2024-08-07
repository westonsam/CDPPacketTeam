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


string unmodifyString (string cdp, int position){
    char aByte;
    if(cdp[position] > 57 && cdp[position+1] > 57 ){
        aByte = (((cdp[position] -55) << 4) &0xF0) | ((cdp[position+1]-55) & 0x0F);
        //aByte = (cdp[position] - 55)  << 4;
    }
    else if (cdp[position] > 57 && cdp[position+1] <57)
    {
        aByte = (((cdp[position] -55) << 4) &0xF0) | ((cdp[position+1]-48) & 0x0F);
    }
    else if (cdp[position] < 57 && cdp[position+1] >57)
    {
        aByte = (((cdp[position] -48) << 4) &0xF0) | ((cdp[position+1]-55) & 0x0F);
    }
    else{
        aByte = (((cdp[position]-48) << 4) & 0xF0) | ((cdp[position+1]-48) &0x0F);
        //aByte = (cdp[position] -48) << 4;
    }
    cout << aByte << endl;
    cdp[position]= aByte;
    cdp.erase(position+1, 1);
    return cdp;

}


int main()
{

    // Instantiate Packet Object
    Packet dp;

    vector<uint8_t> packetRecieved;
    //packetRecieved = {'P', 'A', 'P', 'A' , 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A' , 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A' , 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A', 'P', 'A'};
    string CDP = "PAPA0001MAMA0003ADB3100200ACE57C36Test Data String";

    for(int i = 0; i < 7; i++)
    {
        CDP = unmodifyString(CDP, TOPIC_POS+i);
    }

    packetRecieved = duckutils::convertStringToVector(CDP);
    dp.setBuffer(packetRecieved);

    //gets payload generated
    vector<uint8_t> payload = dp.getBuffer();


   //parses sections of received cdp payload
    vector<string> outputFileData;
    outputFileData = dp.decodePacket(payload);

    CDP = duckutils::convertVectorToString(payload);

    cout << "CDP packet as a string: " << CDP << endl;

    writeFileWebServer("outfile.txt", outputFileData);



    return 0;
}