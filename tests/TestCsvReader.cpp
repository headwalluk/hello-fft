#include <gtest/gtest.h>
#include "CsvReader.h"
#include <fstream>
#include <cstdio>

class CsvReaderTest : public ::testing::Test {
protected:
    std::string temp_file = "test_data.csv";

    void SetUp() override {
        // Create a dummy CSV file
        std::ofstream file(temp_file);
        file << "1.0\n";
        file << "2.5\n";
        file << " \n"; // Empty line
        file << "-0.5\n";
        file << "invalid\n"; // Malformed
        file << "10.0\r\n"; // DOS line ending
        file.close();
    }

    void TearDown() override {
        std::remove(temp_file.c_str());
    }
};

TEST_F(CsvReaderTest, ReadsValidData) {
    auto data = CsvReader::read(temp_file);
    
    // We expect 4 valid numbers: 1.0, 2.5, -0.5, 10.0
    // 'invalid' should be skipped
    // empty line should be skipped
    
    ASSERT_EQ(data.size(), 4);
    EXPECT_DOUBLE_EQ(data[0], 1.0);
    EXPECT_DOUBLE_EQ(data[1], 2.5);
    EXPECT_DOUBLE_EQ(data[2], -0.5);
    EXPECT_DOUBLE_EQ(data[3], 10.0);
}

TEST(CsvReader, ThrowsOnMissingFile) {
    EXPECT_THROW(CsvReader::read("non_existent.csv"), std::runtime_error);
}
