#pragma once

#include <string>
#include <vector>
#include <stdexcept>

class CsvReader {
public:
    /**
     * Reads a CSV file and extracts amplitude data.
     * Assumes one column of numeric data.
     * @param filepath Path to the CSV file.
     * @return Vector of amplitude values.
     * @throws std::runtime_error if file cannot be opened or parsed.
     */
    static std::vector<double> read(const std::string& filepath);
};
