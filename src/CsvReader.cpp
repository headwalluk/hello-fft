#include "CsvReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<double> CsvReader::read(const std::string& filepath) {
    std::vector<double> data;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        
        // Assuming simple CSV with one value per line or taking the first value
        // Handling potential whitespace and comments could be added here
        if (std::getline(ss, cell, ',')) {
             try {
                // Skip empty lines
                if (cell.empty()) continue;
                
                // Remove potential trailing carriage return
                if (cell.back() == '\r') cell.pop_back();
                
                data.push_back(std::stod(cell));
             } catch (const std::exception& e) {
                 // Ignore malformed lines or headers for now, or throw
                 // std::cerr << "Warning: skipping malformed line: " << line << std::endl;
             }
        }
    }

    return data;
}
