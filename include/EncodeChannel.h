#ifndef ENCODECHANNEL_H
#define ENCODECHANNEL_H

#include <vector>

class EncodeChannel{

public:
    std::vector<int> encodeSrc;
    std::vector<int> encodeChannel;
    std::vector<int> decodeChannel;
    
    EncodeChannel(std::vector<int>& bits) : encode(bits) {};
    ~EncodeChannel(){};

    void HammingEncode();
    void HammingDecode();

    

private:

}

#endif