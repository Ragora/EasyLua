# Kiaro Game Engine CMake Build File
# This software is licensed under the Draconic Free License Version 1.
# Please refer to LICENSE.txt for more information.
# Copyright (c) 2014 Draconic Entertainment

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (Subtables)

file(GLOB_RECURSE LUA_SOURCES "../../source/*.c")

LINK_DIRECTORIES("../../bin/subtables")
INCLUDE_DIRECTORIES ("../../include" "../../source")
ADD_EXECUTABLE ("../../bin/subtables" "main.cpp" ${LUA_SOURCES})