#include <vector>
#include <utility>
#include <stdio.h>
#include <cstdint>

#ifndef TRSIGNAL_H
#define TRSIGNAL_H


class Signal{

public:
    
    std::vector<uint8_t> signal;

    Signal(const std::vector<uint8_t>& signal) : signal(signal) { };
    Signal(std::vector<uint8_t>&& signal) : signal(std::move(signal)) { };

    int len() { return signal.size(); };

private:


};



#endif
