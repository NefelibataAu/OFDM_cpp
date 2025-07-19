#include "utils.h"
#include <cstdint>

// template<typename T>
// void printSignal(std::vector<T>& signal){
//     for(int i = 0; i < signal.size(); i++){
//         std::cout << signal[i] << " " ;
//     }
//     std::cout << "\n";
// }


void printSignal(std::vector<uint8_t>& signal){
    for(int i = 0; i < signal.size(); i++){
        std::cout << static_cast<int>(signal[i]) << " " ;
    }
    std::cout << "\n";
}