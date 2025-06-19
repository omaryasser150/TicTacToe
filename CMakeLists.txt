cmake_minimum_required(VERSION 3.16)
project(AdvancedTicTacToe VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt's MOC (Meta-Object Compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Core library sources
set(CORE_SOURCES
    Auth.cpp
    Game.cpp
    AI.cpp
    History.cpp
)

set(CORE_HEADERS
    Auth.h
    Game.h
    AI.h
    History.h
)

# Create core library
add_library(TicTacToeCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Main application sources (GUI components will be added later)
set(APP_SOURCES
    main.cpp
    # StartupWindow.cpp    # Uncomment when GUI files are available
    # GameWindow.cpp       # Uncomment when GUI files are available
    # HistoryWindow.cpp    # Uncomment when GUI files are available
)

set(APP_HEADERS
    # StartupWindow.h      # Uncomment when GUI files are available
    # GameWindow.h         # Uncomment when GUI files are available
    # HistoryWindow.h      # Uncomment when GUI files are available
)

# Create main application executable
add_executable(AdvancedTicTacToe ${APP_SOURCES} ${APP_HEADERS})

# Link libraries to main application
target_link_libraries(AdvancedTicTacToe 
    TicTacToeCore
    Qt6::Core
    Qt6::Widgets
)

# Test executable
option(BUILD_TESTS "Build test executable" ON)

if(BUILD_TESTS)
    # Test sources
    set(TEST_SOURCES
        tests/test_main.cpp
        tests/test_auth.cpp
        tests/test_game.cpp
        tests/test_ai.cpp
        tests/test_history.cpp
    )

    # Create test executable
    add_executable(RunTests ${TEST_SOURCES})
    
    # Link libraries to test executable
    target_link_libraries(RunTests TicTacToeCore)
    
    # Set test executable properties
    set_target_properties(RunTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )
    
    # Add custom target to run tests
    add_custom_target(run_tests
        COMMAND RunTests
        DEPENDS RunTests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running tests..."
    )
endif()

# Installation
install(TARGETS AdvancedTicTacToe
    RUNTIME DESTINATION bin
)

# CPack configuration for packaging
include(CPack)
set(CPACK_PACKAGE_NAME "AdvancedTicTacToe")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced Tic Tac Toe Game with AI")
set(CPACK_PACKAGE_VENDOR "TicTacToe Games")

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(AdvancedTicTacToe PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Copy Qt DLLs on Windows
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QT_BINARY_DIR ${Qt6_DIR}/../../../bin)
    else()
        set(QT_BINARY_DIR ${Qt6_DIR}/../../../bin)
    endif()
    
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(AdvancedTicTacToe PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
    )
endif()

# Debug configuration
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

# Print configuration info
message(STATUS "Building Advanced Tic Tac Toe ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt version: ${Qt6_VERSION}")
message(STATUS "Build tests: ${BUILD_TESTS}")