#include "EncodeSrc.h"

void EncodeSrc::RunLengthEnocde(){

    if( orignSrc.empty() ) return;
    
    uint8_t current = orignSrc[0];
    uint8_t count = 1;

    for(int i = 1; i < orignSrc.size(); i++){
        if( current == orignSrc[i] ){
            count++;
        }
        else{
            runencode.emplace_back(current, count);
            current = orignSrc[i];
            count = 1;
        }  
    }
    runencode.emplace_back(current, count);
}