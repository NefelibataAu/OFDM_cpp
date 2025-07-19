#include <vector>
#include <cstdint>

#ifndef ENCODESRE_H
#define ENCODESRE_H

class EncodeSrc{

public:
    
    std::vector<uint8_t> orignSrc;
    std::vector<std::pair<uint8_t, uint8_t>> runencode;
    // std::vector<uint8_t> encodeSrc;

    EncodeSrc(std::vector<uint8_t>& bits) : orignSrc(bits), runencode(){ };
    ~EncodeSrc(){};

    // void Huffman();
    void RunLengthEnocde();


private:

    
};

#endif