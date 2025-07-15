#include <TRsignal.h>

int main(){

    std::vector<int> signal = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    Signal transmit(signal);
    transmit.printSignal();

}