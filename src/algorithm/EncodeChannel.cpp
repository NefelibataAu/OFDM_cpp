#include "EncodeChannel.h"

void EncodeChannel::HammingEncode(){

    for(auto byte : encodeSrc){
        for(int i = 0; i < 2; i++){
            uint8_t nibble = (byte >> i * 4) & 0x0F;
            
            bool d1 = ( nibble >> 3) & 1;
            bool d2 = ( nibble >> 2) & 1;
            bool d3 = ( nibble >> 1) & 1;
            bool d4 = nibble & 1;

            bool p1 = d1 ^ d2 ^ d4;
            bool p2 = d1 ^ d3 ^ d4;
            bool p3 = d2 ^ d3 ^ d4;

            uint8_t HammingByte = (p1 << 6) | (p2 << 5) | (d1 << 4) |
            (p3 << 3) | (d2 << 2) | (d3 << 1) | d4;

            encodeChannel.emplace_back(HammingByte);
        }
    }
}