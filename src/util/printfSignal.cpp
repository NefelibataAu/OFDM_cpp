#include "utils.h"

template<typename T>
void printSignal(std::vector<T>& signal){
    for(int i = 0; i < signal.size(); i++){
        std::cout << signal[i] << " " ;
    }
    std::cout << "\n";
}