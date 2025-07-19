
#include <vector>
#include <string>
#include <complex>
#include <random>

#ifndef CHANNEL_H
#define CHANNEL_H

class Channel{

public:
    std::string flag;
    std::vector<std::complex<double>> tsignal;
    std::vector<std::complex<double>> rsignal;

    Channel(std::vector<std::complex<double>>& signal) : tsignal(signal), rsignal() {};
    ~Channel(){};

    void AWGN();
    void Rayleigh();
    void Rician();

private:

};

#endif