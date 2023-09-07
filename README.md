# OFDM_cpp
Description: A personal project intended to investigate BER performance of vanilla OFDM system and the effect of error correction coding.

## Getting Started
### How to set up?
The author of this project uses C++(14) and MATLAB(R_2022b). Addtional package manager such as Homebrew is used but not required for program execution. CMake is recommended for simpler building process.
### Dependencies

1. fftw3: Fast Fourier Transform library
2. MATLAB Data API

### Cmake commands
mkdir build

cd build

cmake ../

make &or& make --build .

./ofdm

### Debug Info
The author of this project uses the macOS built-in lldb debugger and AddressSanitizer. 

To start debugging, use 'lldb ofdm' after cmake. Use "b ${file_name} : ${line_number}" to set a breakpoint. After setting breakpoints, use "r" to run the debugger.

* s: step in
* c: continue
* n: step over 
* p ${var_name}: print variable
* q: quit debugger
### List of questions and thoughts

1. MATLAB does column-major reshape operations, while C++ performs row-major operation. Be cautious!
2. filter() in ofdm.cpp does convolution by brute force. Truncate the last $(CP_length_) elements to get the desired behavior.
3. generateAWGN() now uses std::mt19937() random number generator. This is the key to ensure similar behavior with respect to MATLAB implementation. Using default rng would, for example, results in double symbol errors when running bpsk for 5000 iterations.
4. Why would not multiplying channel impulse response (CIR) by sqrt(rho) significantly impact 16qam but not bpsk or qpsk?
   
   Interesting observation: compensating for both mag and phase of a rayleigh channel is the same as compensating for phase only.

5. X-axis: Is it SNR or Eb/N0 when average bit energy is the same across different modulation schemes? what is the relationship between SNR and EbN0?
6. What is the purpose of cyclic prefix? 

   Purpose of cyclic prefix: we have managed to restrict the inter-symbol interference to only symbols in the same block and eliminated the effect of inter-block interference. Adding cyclic prefix is essentially a circular convolution between input x and channel impulse response (CIR) h.

7. Why is the Multi-carrier modulation signal represented as $x(t) = \Sigma_k{X_ke^{j2\pi k F_0 t}}$ ?

    Let us represent the $k_{th}$ transmitted symbol as $X_k$ and its corresponding subcarrier as $e^{j2\pi kF_0t}$ Hence, the received symbol without noise is $y(t) = \Sigma_{k} X_ke^{j2\pi kF_0t}$. On the receiver side, to extract the $l_{th}$
    symbol, $\tilde{X_l} = F_0 \int_{0}^{1/F_0}e^{-j2\pi kF_0t}y(t)dt$ where $F_0 = \frac{B}{N}$.

    It is worth noting that IFFT operation only generates the samples of the actual signal in time domain at specific time stamps, which is sampled at Nyquist rate $f_s = 2 f_{max}$. As opposed to MCM, where we need to generate all the symbols and modulators, OFDM replaces all that with simple IFFT and FFT operations.

8. Is having multiple pilot subcarriers that span the bandwidth a mean to estimate the channel?

    Yes, having multiple pilot subcarriers that span the bandwidth in an OFDM (Orthogonal Frequency Division Multiplexing) system is indeed a common method to estimate the channel's frequency response. These pilot subcarriers serve as known reference points in the transmitted signal, allowing the receiver to estimate how the channel affects the signal at various frequencies within the bandwidth.

9. Pilot symbols vs. Pilot subcarriers
In the system that I am using right now, these two terms have different meanings. In a single iteration, 10 ofdm symbols with each symbol spread across 16 separate subcarriers are transmitted through a channel. Since these 

10. #9 leads to this question. Does a block-fading model reflect the reality? 
> In both cases, you need to interpolate the channel between the pilots you've got. Both cases are typically suboptimal, since they'd only work perfectly for (a) actual block-fading (which is a convenient model, but doesn't look like reality) or (b) for a channel that is perfectly interpolatable from just a few points of observation in frequency (but that would imply you have designed an OFDM system with too many subcarrier, and that has other downsides).
>
```
(10 x 16)
             *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * 
             *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  <---- an OFDM symbol
             *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * 
             *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * 
                                  .
                                  .
                                  .
             *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  * 
             *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *      

                      ^
                      |
                      |
                      |
                      one subcarrier
```