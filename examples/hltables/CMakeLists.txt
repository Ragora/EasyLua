# CMake lists to build the Method Calling Example
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (HighLevelTables)

file(GLOB_RECURSE LUA_SOURCES "../../source/*.c")

LINK_DIRECTORIES("../../bin/hltables")
INCLUDE_DIRECTORIES ("../../include" "../../source")
ADD_EXECUTABLE ("../../bin/hltables" "main.cpp" ${LUA_SOURCES})
