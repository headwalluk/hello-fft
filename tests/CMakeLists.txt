find_package(GTest REQUIRED)

add_executable(unit_tests
    TestCsvReader.cpp
    TestFftProcessor.cpp
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest_main
    core_lib
)

include(GoogleTest)
