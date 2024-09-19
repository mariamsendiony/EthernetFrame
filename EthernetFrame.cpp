#include "EthernetFrame.h"

uint32_t EthernetFrame::crc32()
{
   
    
    std::vector<uint64_t> data ={preamble, EthernetConfig::DestAddress, EthernetConfig::SourceAddress,payload,etherType};
    uint32_t crc = 0xFFFFFFFF; // Initial value
    for (auto byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; ++i) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320; // Polynomial
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc; // Final XOR value
}

EthernetFrame::EthernetFrame()
{   
    
    payload=0x00;
    destinationMac= EthernetConfig::DestAddress;
    sourceMac= EthernetConfig::SourceAddress;
    preamble=0xAAAAAAAAAAAAAAAB;
    etherType=0xAEFE;
    
}

void EthernetFrame::setpreamble(uint64_t &preamble)
{

    this->preamble=preamble;
}

void EthernetFrame::setpayload(uint64_t &payload)
{
    this->payload=payload;
}

void EthernetFrame::setType(uint16_t &etherType)
{
    this->etherType=etherType;
   
}

void EthernetFrame::setSourceAddr(uint64_t &SourceAddress)
{

   sourceMac=SourceAddress;
}

void EthernetFrame::setDestAddr(uint64_t &DestinationAddress)
{
    destinationMac=DestinationAddress;
}

void EthernetFrame::setIFG(uint64_t &IFG)
{
   this->IFG=IFG;
}

uint64_t EthernetFrame::getPreamble()
{
    return preamble;
}

uint64_t EthernetFrame::getpayload()
{
    return payload;
}

uint64_t EthernetFrame::getType()
{
    return etherType;
}

uint64_t EthernetFrame::getSourceAddr()
{
    return sourceMac;
}

uint64_t EthernetFrame::getDestAddr()
{
    return destinationMac;
}

vector< unsigned int > EthernetFrame::GeneratePacket_Parameters()
{   vector <unsigned int> parameters;

    unsigned int count_IFG_per_Packet = EthernetConfig::MinNumOfIFGsPerPacket;
    unsigned int bytes_per_us= (EthernetConfig::LineRate*1000)/(8);
    unsigned long int bytes_due_stream = ceil(bytes_per_us * ( EthernetConfig::CaptureSizeMs*pow(10,3)));
    //Check 4 bytes alignment
    while(( EthernetConfig::MaxPacketSize+ count_IFG_per_Packet)%4!=0)
    {
        count_IFG_per_Packet+=1;

    }
    unsigned int bytes_per_burst = (count_IFG_per_Packet+ EthernetConfig::MaxPacketSize) * EthernetConfig::BurstSize;
    //cout<<"bytes per burst"<<bytes_per_burst<<"\n";
    unsigned int number_bursts=floor( (EthernetConfig::CaptureSizeMs*pow(10,3))/( EthernetConfig::BurstPeriodicity_us));
    //Knowing that the period between each burst and the next is 100 microseconds,we must maintain the line rate.
    //cout<<"number bursts"<<number_bursts<<"\n";
    double time_duration_burst=(double)bytes_per_burst/(double)(bytes_per_us);
    //after getting the duration of burst, get the ifg duration and maintain the line rate
    //cout<<"time_duration_burst"<<time_duration_burst<<"\n";
    float bytes_in_between_bursts=bytes_per_us*(  EthernetConfig::BurstPeriodicity_us-time_duration_burst);
    //cout<<"bytes_in_between_bursts"<<bytes_in_between_bursts<<"\n";
    unsigned int number_IFG_between_bursts=ceil(bytes_in_between_bursts/ EthernetConfig::MinNumOfIFGsPerPacket);
    unsigned int remaining_bytes_append_at_end=bytes_due_stream-((bytes_in_between_bursts+bytes_per_burst)*number_bursts);
    unsigned int number_IFGs_append_at_end=ceil(remaining_bytes_append_at_end/ EthernetConfig::MinNumOfIFGsPerPacket);
    //std::cout<<"remaining\n"<<remaining_bytes_append_at_end<<"\n";
    //std::cout<<number_IFGs_append_at_end;
    //std::cout<< EthernetConfig::BurstSize;
    
    parameters={count_IFG_per_Packet,number_IFG_between_bursts,number_IFGs_append_at_end,number_bursts, EthernetConfig::BurstSize};
    generate_packet(count_IFG_per_Packet, EthernetConfig::MaxPacketSize);
    return parameters;
  

}
long long convertStrHexa(string hexaStr){
    const int hexaStrLength = hexaStr.length();
    // const short diffLowerCaseToDigits, diffUpperCaseDigits;
    long long res = 0;
    short int hexaCharToDigit;
    if (hexaStrLength <= 2 || ! (hexaStr[0] && (hexaStr[1] == 'x' || hexaStr[1] == 'X')))
        return -1;
    
    for (int i = 2; i < hexaStrLength; i++){ // start from the third element after 0x
        // if (hexaStr[i] == 'x' || hexaStr[i] == 'X')
        //     break; // reached the hexadecimal annotator
        
        if (hexaStr[i] >= 48 && hexaStr[i] <= 57) // that means that the number in the range of [0-9]
        {
            hexaCharToDigit = hexaStr[i] - 48;
        }
        else if (hexaStr[i] >= 97 && hexaStr[i] <= 102) // means in range [a-f]
        {
            hexaCharToDigit = hexaStr[i] - 97 + 10; // 10 is the offset where a starts from 10
        }
        else if (hexaStr[i] >= 65 && hexaStr[i] <= 70)
        {
            hexaCharToDigit = hexaStr[i] - 65 + 10;
        }
        else
        {
            cout << "Incorrect format" << endl;
            return -1;
        }
        res = res << 4;  // 4 shifts equivalent to multiply by 16
        res += hexaCharToDigit;
    }
    return res;
}

string EthernetFrame::intToHex(uint32_t value)
{
    std::ostringstream ss;
    ss << std::hex << value;
    return ss.str();
   
}

string EthernetFrame::generate_packet(int IFG_per_packet, int max_packet_size)
{
    //12 bytes for address,4 for crc,2 ethertype,8 preamble=26
    //To get number of hexadecimal digits,multiply the number of bytes by 2
    string packet;
    int num_zeros_payload=2*(max_packet_size-26);
    int num_zeroes_IFG=2*IFG_per_packet;
    uint32_t crc=crc32();
    packet=intToHex(preamble)+to_string(destinationMac)+to_string(sourceMac)+intToHex(etherType)+std::string(num_zeros_payload, '0')+intToHex(crc)+std::string(num_zeroes_IFG, '0');
    //cout<<packet;
    return packet ;
}

