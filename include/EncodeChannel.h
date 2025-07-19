
#include <vector>
#include <cstdint>

#ifndef ENCODECHANNEL_H
#define ENCODECHANNEL_H

class EncodeChannel{

public:
    std::vector<uint8_t> encodeSrc;
    std::vector<uint8_t> encodeChannel;
    
    EncodeChannel(std::vector<uint8_t>& bits) : encodeSrc(bits), encodeChannel() {};
    ~EncodeChannel(){};

    void HammingEncode();
    // void HammingDecode();

    

private:

};

#endif