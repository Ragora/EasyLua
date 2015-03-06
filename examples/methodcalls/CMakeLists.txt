# CMake lists to build the Method Calling Example
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (MethodCalls)

file(GLOB_RECURSE LUA_SOURCES "../../source/*.c")

LINK_DIRECTORIES("../../bin/methodcalls")
INCLUDE_DIRECTORIES ("../../include" "../../source")
ADD_EXECUTABLE ("../../bin/methodcalls" "main.cpp" ${LUA_SOURCES})