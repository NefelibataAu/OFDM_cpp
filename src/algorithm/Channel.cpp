

#include "Channel.h"

void Channel::AWGN(){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution<double> distribution(0.0, 1.0/sqrt(2.0));

    for(int i = 0 ; i < tsignal.size(); i++){
        std::complex<double> z ( distribution(generator), distribution(generator));
        rsignal[i] = z + tsignal[i];
    }
}