# Core Logic Library (separated for testing)
add_library(core_lib
    CsvReader.cpp
    FftProcessor.cpp
)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core_lib PUBLIC
    ${FFTW3_LIBRARIES}
)
target_include_directories(core_lib PUBLIC
    ${FFTW3_INCLUDE_DIRS}
)

# Main GUI Executable
add_executable(hello-fft
    main.cpp
    MainWindow.cpp
    HistogramWidget.cpp
)

target_link_libraries(hello-fft PRIVATE
    core_lib
    ${GTKMM_LIBRARIES}
)

target_include_directories(hello-fft PRIVATE
    ${GTKMM_INCLUDE_DIRS}
)
