# CMake lists to build the sub table example
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Subtables)

file(GLOB_RECURSE LUA_SOURCES "../../source/*.c")

LINK_DIRECTORIES("../../bin/subtables")
INCLUDE_DIRECTORIES ("../../include" "../../source")
ADD_EXECUTABLE ("../../bin/subtables" "main.cpp" ${LUA_SOURCES})