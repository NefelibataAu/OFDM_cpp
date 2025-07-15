#ifndef TRSIGNAL_H
#define TRSIGNAL_H

#include <vector>

class Signal{

public:
    
    std::vector<int> signal;
    int len;

    Signal(std::vector<int>& signal) : signal(signal) { len = signal.size(); };


private:


};



#endif
