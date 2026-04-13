# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appDoorLeap_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appDoorLeap_autogen.dir\\ParseCache.txt"
  "appDoorLeap_autogen"
  )
endif()
