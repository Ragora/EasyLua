/**
 *  @file main.cpp
 *  @brief Example coding showing the different Lua method calling
 *  features.
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt
 *  at the root of any source distribution for more information.
 */

#include <iostream>

#include <easylua.hpp>

#include <gtest/gtest.h>


TEST(MethodCalls, Basic)
{
    return;

    // Init Lua
    lua_State *lua = luaL_newstate();
    luaL_checkversion(lua);
    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, 0);

    EXPECT_EQ(0, luaL_dofile(lua, "main.lua"));

    std::cout << "---- Initialized Lua " << std::endl;

    // Perform a call and get the number of returned elements
    const unsigned int returnCount = EasyLua::call(lua, "easyLuaMultiParamMultiReturnTest", 1, "Two", 3.14f);
    std::cout << "Return Count: " << returnCount << std::endl;

    /*
        The above code would look like this with straight Lua manipulations:

        const int stackTop = lua_gettop(lua);
        lua_getglobal(lua, "easyLuaMultiParamMultiReturnTest");
        lua_pushinteger(lua, 1);
        lua_pushstring(lua, "Two");
        lua_pushnumber(lua, 3.14f);
        lua_call(lua, 3, LUA_MULTRET);

        const int returnCount = lua_gettop(lua) - stackTop;
    */

    // We need space to read our returns to
    int integerReturnOne = -1;
    float floatReturn = -1;
    std::string stringReturnThree;
    int integerReturnTwo = -1;

    char *stringReturnOne = (char*)malloc(256);
    memset(stringReturnOne, 0x00, 256);
    char stringReturnTwo[256];
    memset(stringReturnTwo, 0x00, 256);

    // The Lua method we called will return an int, float, string, string, string int
    // We use pointers for everything as to prevent usage of stack space values internally
    EasyLua::Utilities::readStack<false>(lua, &integerReturnOne, &floatReturn,
                                  stringReturnOne, 256,
                                  stringReturnTwo, 256,
                                  &stringReturnThree, &integerReturnTwo);

    /*
        The above code would look like this with straight Lua manipulations:

        const int integerReturnOne = luaL_checkinteger(lua, 1);
        const float floatReturn = luaL_checknumber(lua, 2);
        const char *stringReturnOne = luaL_checkstring(lua, 3);
        const char *stringReturnTwo = luaL_checkstring(lua, 4);
        const std::string stringReturnThree = luaL_checkstring(lua, 5);
        const int integerReturnTwo = luaL_checkinteger(lua, 6);
    */

    // Print out the values
    std::cout << "Got Int: " << integerReturnOne << std::endl;
    std::cout << "Got Float: " << floatReturn << std::endl;
    std::cout << "Got String: " << stringReturnOne << std::endl;
    std::cout << "Got String: " << stringReturnTwo << std::endl;
    std::cout << "Got String: " << stringReturnThree << std::endl;
    std::cout << "Got Int: " << integerReturnTwo << std::endl;

    // If any of the types are wrong, readStack will return the index of the first occurrence
    const int stackError = EasyLua::Utilities::readStack<false>(lua, &integerReturnOne, stringReturnOne, 256);

    std::cout << "Stack Error Index: " << stackError << std::endl;

    // We can also catch an exception if we have type mismatches
    try
    {
        EasyLua::Utilities::readStack<true>(lua, stringReturnOne, 256);
    }
    catch (std::runtime_error &e)
    {
        std::cout << "Caught Runtime Exception: " << e.what() << std::endl;
    }

    // Deinit
    lua_close(lua);
    std::cout << "---- Deinitialized Lua" << std::endl;

    free(stringReturnOne);
}
