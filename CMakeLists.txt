# CMakeLists.txt
# Used for building the example code on any CMake supported system.
# This software is licensed under the MIT license. Please refer to
# LICENSE.txt for more information.

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(EasyLua)

FIND_PACKAGE(GTest REQUIRED)

# Configure for GNU Compiler
IF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    # gnu++0x is necessary for GTest on Windows, apparently.
    # We also need to link pthread
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu++0x -lpthread -pthread")

    IF (DEBUG)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    ENDIF (DEBUG)
ENDIF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)


FILE(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
FILE(GLOB_RECURSE LUA_SOURCES "source/lua/*.c")
FILE(GLOB_RECURSE LUA_INCLUDES "source/*.h")
FILE(GLOB_RECURSE EASYLUA_INCLUDES "include/*.hpp")
FILE(GLOB_RECURSE EASYLUA_SOURCES "source/*.cpp")

INCLUDE_DIRECTORIES("source/")
INCLUDE_DIRECTORIES("include/" "source/lua")
ADD_EXECUTABLE (Tests ${TEST_SOURCES} ${LUA_SOURCES} ${LUA_INCLUDES} ${EASYLUA_INCLUDES} ${EASYLUA_SOURCES})
TARGET_LINK_LIBRARIES(Tests ${GTEST_LIBRARIES} "lua5.2")
SET_TARGET_PROPERTIES(Tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
