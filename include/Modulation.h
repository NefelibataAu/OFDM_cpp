#include <vector>
#include <cstdint>
#include <complex>
// #include <math>

#ifndef MODULATION_H
#define MODULATION_H

class Modulation{

public:
    std::vector<uint8_t> encodeChannel;
    std::vector<std::complex<double>> modulSignal;

    Modulation(){};
    ~Modulation(){};

    void ASK();
    void PSK();
    void FSK();
    void QAM();
    void OFDM();
    void OTFS();

private:
    double carry_freq;
    double sample_freq;
    double byte_time;
    double time;
};

#endif