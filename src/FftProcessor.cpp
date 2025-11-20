#include "FftProcessor.h"
#include <cmath>
#include <iostream>

std::vector<double> FftProcessor::computeFft(const std::vector<double>& timeDomainData) {
    if (timeDomainData.empty()) {
        return {};
    }

    int N = timeDomainData.size();
    // Output size for real-to-complex transform is N/2 + 1
    int outSize = N / 2 + 1;

    // Allocate input and output arrays
    double* in = (double*) fftw_malloc(sizeof(double) * N);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * outSize);

    // Create plan
    fftw_plan p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

    // Copy data to input array
    for (int i = 0; i < N; ++i) {
        in[i] = timeDomainData[i];
    }

    // Execute FFT
    fftw_execute(p);

    // Compute magnitudes
    std::vector<double> magnitudes;
    magnitudes.reserve(outSize);

    for (int i = 0; i < outSize; ++i) {
        double re = out[i][0];
        double im = out[i][1];
        magnitudes.push_back(std::sqrt(re * re + im * im));
    }

    // Clean up
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return magnitudes;
}
