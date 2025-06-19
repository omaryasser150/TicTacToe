cmake_minimum_required(VERSION 3.10)
project(AdvancedTicTacToe)

set(CMAKE_CXX_STANDARD 17)

# Game Source
file(GLOB SOURCES "src/*.cpp")

# Add Google Test
enable_testing()
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Main test executable
add_executable(runTests tests/GameTest.cpp ${SOURCES})
target_link_libraries(runTests gtest gtest_main)

# Register tests
add_test(NAME AllTests COMMAND runTests)
