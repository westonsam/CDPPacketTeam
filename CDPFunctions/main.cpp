#include <iostream>
#include <fstream> //for file handling
#include <cstdlib>   // for exit()
#include <string>
#include <cstdint> //used for uint8_t
#include <vector> //includes vectors, similar arrays

using namespace std;

string DDUID;
string DATA;

vector<uint8_t> convertStringToVector (string line);
string convertVectorToString (vector<uint8_t> buffer);
string readFile(string filename);
void writeFile(string filename, vector<uint8_t> RxData);

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
        cout << "Output file failed to open.\n";
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

string processInputFile(string input){
    DDUID = input.substr(input.find("DUID:"), input.find("DATA:"));
    DATA = input.substr(input.find("DATA:"), input.length() - 1);
    cout <<  "PROCESSED:" << endl;
    cout << DDUID << endl;
    cout << DATA << endl;
    return "";
}

int main() {
    string inputData = readFile("infile.txt");
    cout << inputData << endl;
    processInputFile(inputData);
    vector<uint8_t> vector = convertStringToVector(inputData);
    writeFile("outfile.txt", vector);
    
}