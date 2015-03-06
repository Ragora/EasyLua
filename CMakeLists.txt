# CMakeLists.txt
# Used for building the example code on any CMake supported system.
# This software is licensed under the MIT license. Please refer to
# LICENSE.txt for more information.

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(EasyLua)

# Configure for GNU Compiler
IF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    # gnu++0x is necessary for GTest on Windows, apparently.
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu++0x")
   
    IF (DEBUG)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    ENDIF (DEBUG)
ENDIF (CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)

# Handle our build options

ADD_SUBDIRECTORY("examples/methodcalls")
ADD_SUBDIRECTORY("examples/subtables")
