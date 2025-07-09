#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>
#include <string>

class Channel{

public:
    std::string flag;
    std::vector<float> tsignal;
    std::vector<float> rsignal;

    Channel(std::vector<float>& signal) : signal(siganl) {};
    ~Channel(){};

    void AWGN();
    void Rayleigh();
    void Rician();
    

private:

}

#endif