#ifndef RF
#define RF

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


// Define a struct to hold Ethernet configuration parameters
 struct EthernetConfig {
    static unsigned int LineRate;
    static double CaptureSizeMs;
    static unsigned int MinNumOfIFGsPerPacket;
    static uint64_t DestAddress;
    static uint64_t SourceAddress;
    static unsigned int MaxPacketSize;
    static unsigned int BurstSize;
    static double BurstPeriodicity_us;
};

     unsigned int LineRate;
     double CaptureSizeMs;
     unsigned int MinNumOfIFGsPerPacket;
     uint64_t DestAddress;
     uint64_t SourceAddress;
     unsigned int MaxPacketSize;
     unsigned int BurstSize;
     double BurstPeriodicity_us;

    

// Function to read Ethernet configuration from a file
  bool readEthernetConfig(const std::string& filename, EthernetConfig& ethConfig) {
    std::ifstream inputFile(filename);
    std::string line;
    std::vector<std::string> myVector;
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << std::endl;
        return false;
    }
    while (getline(inputFile, line)) {
        std::cout << line << std::endl;
        size_t pos = line.find('=');
        size_t second_space = line.find(pos + 2,' ');
        

            if (pos != std::string::npos) {
                std::string value = line.substr(pos + 2,second_space-(pos + 2));
                //std::cout <<"value"<< value << std::endl;
                myVector.push_back(value);
            }
            
    }

    ethConfig.LineRate=stoi(myVector[0]);
    ethConfig.CaptureSizeMs=stoi(myVector[1]);
    ethConfig.MinNumOfIFGsPerPacket=stoi(myVector[2]);
    ethConfig.DestAddress=stoull((myVector[3]).erase(0,2));
    ethConfig.SourceAddress=stoull(myVector[4].erase(0,2));
    ethConfig.MaxPacketSize=stoi(myVector[5]);
    ethConfig.BurstSize=stoi(myVector[6]);
    ethConfig.BurstPeriodicity_us=stoi(myVector[7]);
  

    // Close the file
    inputFile.close();
    return true;
}
#endif

/*int main() {
    
    EthernetConfig ethConfig;

    
    readEthernetConfig("first_milestone.txt", ethConfig);
        
    




    return 0;
}*/
