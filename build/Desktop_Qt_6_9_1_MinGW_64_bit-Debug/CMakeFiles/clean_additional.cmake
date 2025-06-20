# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RunTests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RunTests_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TicTacToeCore_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TicTacToeCore_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TicTacToe_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TicTacToe_autogen.dir\\ParseCache.txt"
  "RunTests_autogen"
  "TicTacToeCore_autogen"
  "TicTacToe_autogen"
  )
endif()
