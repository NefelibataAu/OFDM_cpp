#include <TRsignal.h>
#include <utils.h>

int main(){

    std::vector<uint8_t> signal = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    Signal transmit(signal);
    printSignal(transmit.signal);

}