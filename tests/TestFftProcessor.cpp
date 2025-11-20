#include <gtest/gtest.h>
#include "FftProcessor.h"
#include <cmath>

TEST(FftProcessorTest, HandlesEmptyInput) {
    std::vector<double> empty;
    auto result = FftProcessor::computeFft(empty);
    EXPECT_TRUE(result.empty());
}

TEST(FftProcessorTest, DcComponent) {
    // Constant signal should have peak at 0Hz
    std::vector<double> data(8, 1.0); // 8 samples, all 1.0
    auto result = FftProcessor::computeFft(data);
    
    // Size should be N/2 + 1 = 5
    ASSERT_EQ(result.size(), 5);
    
    // DC component (index 0) should be N * amplitude = 8 * 1 = 8.0
    EXPECT_NEAR(result[0], 8.0, 1e-5);
    
    // Other components should be 0
    for (size_t i = 1; i < result.size(); ++i) {
        EXPECT_NEAR(result[i], 0.0, 1e-5);
    }
}

TEST(FftProcessorTest, SineWave) {
    // Create a sine wave
    int N = 16;
    std::vector<double> data(N);
    double freq = 2.0; // 2 cycles per N samples
    
    for (int i = 0; i < N; ++i) {
        data[i] = std::sin(2.0 * M_PI * freq * i / N);
    }
    
    auto result = FftProcessor::computeFft(data);
    
    // Peak should be at index 2
    // Magnitude for real DFT is N/2 for the peak (at non-DC/Nyquist frequencies)
    // 16/2 = 8.0
    EXPECT_NEAR(result[2], 8.0, 1e-5);
    
    // Check neighbors are small (spectral leakage might be non-zero if not integer cycles, 
    // but here it is integer cycles)
    EXPECT_NEAR(result[1], 0.0, 1e-5);
    EXPECT_NEAR(result[3], 0.0, 1e-5);
}
