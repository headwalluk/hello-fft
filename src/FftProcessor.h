#pragma once

#include <vector>
#include <complex>
#include <fftw3.h>

class FftProcessor {
public:
    /**
     * Computes the FFT magnitude spectrum of the input signal.
     * @param timeDomainData The input amplitude data.
     * @return A vector containing the magnitude of each frequency bin.
     *         The size will be N/2 + 1, where N is the input size.
     */
    static std::vector<double> computeFft(const std::vector<double>& timeDomainData);
};
