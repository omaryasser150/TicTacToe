cmake_minimum_required(VERSION 3.16)
project(AdvancedTicTacToe VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 first
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Set Qt automation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# All source files
set(SOURCES
    src/Auth.cpp
    src/Game.cpp
    src/AI.cpp
    src/History.cpp
    src/GameModeWindow.cpp
    src/LoginWindow.cpp
    src/MainWindow.cpp
    src/PlayerVsPlayerWindow.cpp
    src/RegisterWindow.cpp
    src/StartupWindow.cpp
    src/main.cpp
)

# All header files
set(HEADERS
    Header/Auth.h
    Header/Game.h
    Header/AI.h
    Header/History.h
    Header/GameModeWindow.h
    Header/LoginWindow.h
    Header/MainWindow.h
    Header/PlayerVsPlayerWindow.h
    Header/RegisterWindow.h
    Header/StartupWindow.h
)

# Documentation files
set(DOCUMENTATION_FILES
    Documents/SRS.docx
    Documents/SDS.docx
    Documents/Testing_Documentation.docx
)

# Add documentation files to the project (they will appear in IDEs)
add_custom_target(Documentation
    SOURCES ${DOCUMENTATION_FILES}
    COMMENT "Documentation files"
)

# Install documentation files
install(FILES ${DOCUMENTATION_FILES}
    DESTINATION doc
    COMPONENT documentation
)

# Create core library
add_library(TicTacToeCore STATIC ${CORE_SOURCES} ${CORE_HEADERS}
)

# Add include directory for core library
target_include_directories(TicTacToeCore PUBLIC Header)

# Link Qt to core library (needed because headers use Qt classes)
target_link_libraries(TicTacToeCore Qt6::Core Qt6::Widgets)

# Create main executable
add_executable(TicTacToe ${GUI_SOURCES} ${GUI_HEADERS})

# Add include directory for main executable
target_include_directories(TicTacToe PRIVATE Header)

# Link Qt libraries to main executable
target_link_libraries(TicTacToe 
    TicTacToeCore
    Qt6::Core 
    Qt6::Widgets
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
	    tests/test_integration.cpp	
        )

        # Create test executable
        add_executable(RunTests ${TEST_SOURCES})
        
        # Add include directory for tests
        target_include_directories(RunTests PRIVATE Header)
        
        # Link libraries to test executable
        target_link_libraries(RunTests TicTacToe GTest::GTest GTest::Main)
        
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
        
        message(STATUS "Google Test found - tests will be built")
    else()
        message(STATUS "Google Test not found - skipping tests")
        set(BUILD_TESTS OFF)
    endif()
endif()

# Debug configuration
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

# Print configuration info
message(STATUS "Building Advanced Tic Tac Toe ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Documentation files: ${DOCUMENTATION_FILES}")