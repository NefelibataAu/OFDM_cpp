#ifndef ENCODESRE_H
#define ENCODESRE_H

#include  <vector>


class EncodeSrc{

public:
    
    std::vector<int> orignSrc;
    std::vector<int> encodeSrc;

    EncodeSrc(std::vector<int>& bits) : orignSrc(bits), encodeSrc(){ };
    ~EncodeSrc(){};

    void Huffman();


private:

    
};

#endif