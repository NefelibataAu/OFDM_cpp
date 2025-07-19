#include  "Modulation.h"

void Modulation::ASK(){
    int sample_number = static_cast<int>(sample_freq * byte_time);
    for(auto byte : encodeChannel){
        int ask = static_cast<int>(byte);
        for(int i = 0; i < sample_number; i++){
            double real = ask * cos(2 * M_PI * carry_freq * (i / sample_freq + time));
            double imag = ask * sin(2 * M_PI * carry_freq * (i / sample_freq + time));
            std::complex<double> modul(real, imag);
            modulSignal.emplace_back(modul);
        }
        time +=  byte_time;
    }
}