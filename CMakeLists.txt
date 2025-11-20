cmake_minimum_required(VERSION 3.10)
project(hello-fft VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
pkg_check_modules(FFTW3 REQUIRED fftw3)

# Main source directory
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(tests)

# Custom clean target that can be run from project root
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMENT "Cleaning all build artifacts"
)
