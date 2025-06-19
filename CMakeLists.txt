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

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# Enable Qt MOC (Meta-Object Compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Core library sources
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

# GUI sources
set(GUI_SOURCES
    GameModeWindow.cpp
    LoginWindow.cpp
    MainWindow.cpp
    PlayerVsPlayerWindow.cpp
    RegisterWindow.cpp
    StartupWindow.cpp
    main.cpp
)

set(GUI_HEADERS
    GameModeWindow.h
    LoginWindow.h
    MainWindow.h
    PlayerVsPlayerWindow.h
    RegisterWindow.h
    StartupWindow.h
)

# Create core library
add_library(TicTacToeCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Add include directory for headers
target_include_directories(TicTacToeCore PUBLIC Header)

# Link Qt to core library if needed
target_link_libraries(TicTacToeCore Qt6::Core Qt6::Sql)

# Create GUI executable
add_executable(AdvancedTicTacToe ${GUI_SOURCES} ${GUI_HEADERS})

# Add include directory for GUI
target_include_directories(AdvancedTicTacToe PRIVATE Header)

# Link libraries to GUI executable
target_link_libraries(AdvancedTicTacToe 
    TicTacToeCore
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Sql
)

# Set executable properties
set_target_properties(AdvancedTicTacToe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    WIN32_EXECUTABLE TRUE  # For Windows GUI app (removes console window)
)

# Test executable
option(BUILD_TESTS "Build test executable" ON)

if(BUILD_TESTS)
    # Find Google Test
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        # Test sources
        set(TEST_SOURCES
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
        target_link_libraries(RunTests TicTacToeCore GTest::GTest GTest::Main)
        
        # Set test executable properties
        set_target_properties(RunTests PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        
        # Enable testing
        enable_testing()
        
        # Add test to CTest
        add_test(NAME AllTests COMMAND RunTests)
        
        # Add custom target to run tests
        add_custom_target(run_tests
            COMMAND RunTests
            DEPENDS RunTests
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running tests..."
        )
        
        message(STATUS "Google Test found - building tests")
    else()
        message(WARNING "Google Test not found - skipping tests")
        set(BUILD_TESTS OFF)
    endif()
endif()

# Debug configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Custom target to run the GUI application
add_custom_target(run_gui
    COMMAND AdvancedTicTacToe
    DEPENDS AdvancedTicTacToe
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Tic Tac Toe GUI..."
)

# Install target (optional)
install(TARGETS AdvancedTicTacToe
    RUNTIME DESTINATION bin
)

# Print configuration info
message(STATUS "Building Advanced Tic Tac Toe ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Qt6 version: ${Qt6_VERSION}")

# Print build targets
message(STATUS "Available targets:")
message(STATUS "  AdvancedTicTacToe - Main GUI application")
if(BUILD_TESTS)
    message(STATUS "  RunTests - Unit tests")
    message(STATUS "  run_tests - Run unit tests")
endif()
message(STATUS "  run_gui - Run the GUI application")
message(STATUS "  install - Install the application")