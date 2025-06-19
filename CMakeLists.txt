cmake_minimum_required(VERSION 3.16)
project(AdvancedTicTacToe VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Core library sources (updated paths)
set(CORE_SOURCES
    src/Auth.cpp
    src/Game.cpp
    src/AI.cpp
    src/History.cpp
)

set(CORE_HEADERS
    Header/Auth.h
    Header/Game.h
    Header/AI.h
    Header/History.h
)

# Create core library
add_library(TicTacToeCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Add include directory for headers
target_include_directories(TicTacToeCore PUBLIC Header)

# Test executable
option(BUILD_TESTS "Build test executable" ON)

if(BUILD_TESTS)
    # Test sources (updated paths)
    set(TEST_SOURCES
	tests/test_main.cpp
        tests/test_auth.cpp
        tests/test_game.cpp
        tests/test_ai.cpp
        tests/test_history.cpp
    )

    # Create test executable
    add_executable(RunTests ${TEST_SOURCES})
    
    # Add include directory for tests
    target_include_directories(RunTests PRIVATE Header)
    
    # Link libraries to test executable
    target_link_libraries(RunTests TicTacToeCore)
    
    # Set test executable properties
    set_target_properties(RunTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    # Add custom target to run tests
    add_custom_target(run_tests
        COMMAND RunTests
        DEPENDS RunTests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running tests..."
    )
endif()

# Debug configuration
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

# Print configuration info
message(STATUS "Building Advanced Tic Tac Toe Tests ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")