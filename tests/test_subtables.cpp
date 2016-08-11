/**
 *  @file main.cpp
 *  @brief Source file showing
 *
 *  This software is licensed under the MIT license. Refer to LICENSE.txt for
 *  more information.
 *
 *  @date 8/10/2016
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Robert MacGregor
 */

#include <iostream>

#include <easylua.hpp>

#include <gtest/gtest.h>

TEST(Subtables, Basic)
{
    // Init Lua
    lua_State *lua = luaL_newstate();
    luaL_checkversion(lua);
    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, 0);

    EXPECT_EQ(0, luaL_dofile(lua, "main.lua"));

    /*
        The call below performs the following Lua call:

        easyLuaMultiTables(
            { Test = 3 }, "One", 2, "Three", 4.12, "Five", { Six = 7, Eight = { Nine = 10 }}, { Ten = 11 }, 12,
            { Thirteen = 14 })

        However, the resolution of the required Lua code here is not resolved at runtime. The reason for the awkward
        system here is that it all operates on templates to essentially act as a compiler within your compiler to inline
        the required Lua calls at this position in your generated binary. It is not quite as efficient as Human written
        code (or code generated outside of your codebase) due to the way call resolution works, but the performance
        hit should be minor.
    */

    const unsigned int returnCount = EasyLua::call(lua, "easyLuaSubTables",
        // Push a table to the stack
        EasyLua::Utilities::Table<1>(lua, "Test", 3),
        // Push some normal variables after that and then a table
        "One", 2, "Three", 4.12f, "Five", EasyLua::Utilities::Table<1>(lua, "Six", 7,
            // Map Eight to a Table
            "Eight", EasyLua::Utilities::Table<0>(lua, "Nine", 10,
                // And then another subtable because why not
                "Another", EasyLua::Utilities::Table<-1>(lua, "Table", 50))),

        // Finish up the call
        EasyLua::Utilities::Table<1>(lua, "Ten", 11), 12, EasyLua::Utilities::Table<1>(lua, "Thirteen", 14));


   // EXPECT_EQ(1, returnCount);

    bool success = false;
    EasyLua::Utilities::readStack<false>(lua, &success);

    EXPECT_TRUE(success);

    // Deinit
    lua_close(lua);
}
