cmake_minimum_required(VERSION 3.27)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        number_tests
        number_tests.cpp
)

target_link_libraries(
        number_tests
        int2023_t
        GTest::gtest_main
)

target_include_directories(number_tests PUBLIC "${PROJECT_SOURCE_DIR}/lib")

include(GoogleTest)

gtest_discover_tests(number_tests)