
#include "EthernetFrame.cpp"

    unsigned int  EthernetConfig:: LineRate;
    double  EthernetConfig::CaptureSizeMs;
    unsigned int  EthernetConfig::MinNumOfIFGsPerPacket;
    uint64_t  EthernetConfig::DestAddress;
    uint64_t  EthernetConfig::SourceAddress;
    unsigned int  EthernetConfig::MaxPacketSize;
    unsigned int  EthernetConfig::BurstSize;
    double  EthernetConfig::BurstPeriodicity_us;







int main(int argc, char* argv[]) {
    if (argc != 3) {
         cerr << "Usage: " << argv[0] << " <first_milestone.txt> <output_file>\n";
        return 1;
    }
    const char* inputFileName = argv[1];
    const char* outputFileName = argv[2];

    // Open input and output files
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile) {
         cerr << "Error opening files.\n";
        return 1;
    }

    EthernetConfig ethConfig;
    readEthernetConfig(argv[1], ethConfig);

    EthernetFrame ethernet;
    vector<unsigned int>out;
    string packet;
    string IFG;
    int count=0;
    out=ethernet.GeneratePacket_Parameters();
    packet=ethernet.generate_packet(out[0],EthernetConfig::MaxPacketSize);
    IFG=std::string(2*EthernetConfig::MinNumOfIFGsPerPacket, '0');
    ofstream myfile( argv[2]); // Create and open a text file
    if (myfile.is_open()) {
        myfile << "Writing this to a file.\n"; // Write to the file
        for(unsigned int i=0;i<out[3];i++){
            for(unsigned int j=0;j<out[4];j++){
                myfile << packet << '\n';
            }
            count++;
            myfile << "Writing this to a file the burst number"<<count<<'\n'; 
            for(unsigned int k=0;k<out[1];k++){
                myfile<<IFG<<'\n';

            }
                

        }
    if(out[2]>0)
    {
        for(unsigned int i=0;i<out[2];i++)
        {
            myfile<<IFG<<'\n';
        }
    }
        // Close the file
        myfile.close();

        cout << "Data written to output file.\n";
        
    } else {
        cout << "Unable to open file";
    }
    

    return 0;

}