#ifndef MODULATION_H
#define MODULATION_H

#include <vector>

class Modulation{

public:
    std::vector<int> encodeChannel;
    std::vector<float> modulSignal;
    std::vector<int> demodulSignal;

    Modulation(){};
    ~Modulation(){};

    void ASK();
    void PSK();
    void FSK();
    void QAM();
    void OFDM();
    void OTFS();

    

private:

}

#endif