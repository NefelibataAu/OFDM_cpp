#ifndef ENCODESRE_H
#define ENCODESRE_H

#include  <vector>


class EncodeSrc{

public:
    
    std::vector<int> orignSrc;
    std::vector<int> encodeSrc;
    std::vector<int> decodeSrc;

    Encode(std::vector<int>& bits):orignSrc(bits){};
    ~Encode(){};
    void Huffman();


private:

    
}

#endif