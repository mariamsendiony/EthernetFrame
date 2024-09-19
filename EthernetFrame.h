#ifndef EF
#define EF

#include "Readfile.h"

class EthernetFrame{
    private:
    // Member variables (frame components)
    uint64_t preamble;
    uint64_t destinationMac;
    uint64_t sourceMac;
    uint16_t etherType;
    uint64_t payload; 
    uint64_t crc;
    uint64_t IFG;
    public:
    EthernetFrame();
    uint32_t crc32();
    void setpreamble(uint64_t &preamble);
    void setpayload(uint64_t &payload);
    void setType(uint16_t &etherType);
    void setSourceAddr(uint64_t &SourceAddress);
    void setDestAddr(uint64_t &DestinationAddress);
    void setIFG(uint64_t &IFG);
    uint64_t getPreamble();
    uint64_t getpayload();
    uint64_t getType();
    uint64_t getSourceAddr();
    uint64_t getDestAddr();
    vector<unsigned int> GeneratePacket_Parameters();
    long long convertStrHexa(string hexaStr);
    string intToHex(uint32_t value);
    string generate_packet(int IFG_per_packet,int max_packet_size);


};
#endif