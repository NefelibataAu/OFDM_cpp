#include <vector>
#include <utility>
#include <stdio.h>

#ifndef TRSIGNAL_H
#define TRSIGNAL_H


class Signal{

public:
    
    std::vector<int> signal;

    Signal(const std::vector<int>& signal) : signal(signal) { };
    Signal(std::vector<int>&& signal) : signal(std::move(signal)) { };

    int len() { return signal.size(); };

private:


};



#endif
