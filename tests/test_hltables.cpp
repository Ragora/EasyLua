/**
 *  @file main.cpp
 *  @brief Example coding showing the usage of the high level
 *  tables interface.
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

TEST(HLTables, Basic)
{
    // Init Lua
    lua_State *lua = luaL_newstate();
    luaL_checkversion(lua);
    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, 0);

    EXPECT_EQ(0, luaL_dofile(lua, "main.lua"));

    // Init and setup our upper level high level table
    EasyLua::Table* table = new EasyLua::Table();

    {
        EXPECT_NO_THROW(table->set("One", 2.0f));
        EXPECT_NO_THROW(table->set("Three", 4.14f));
    }

    // Init and setup a table connected to the upper level high level table
    EasyLua::Table* subTable = new EasyLua::Table();

    {
        EXPECT_NO_THROW(subTable->set("Six", 7));
        EXPECT_NO_THROW(subTable->set("Eight", 9.14f));
        EXPECT_NO_THROW(table->setTable("Five", *subTable));
    }


    // Init and setup a table connected to the table connected to our high level table
    EasyLua::Table* subSubTable = new EasyLua::Table();

    {
        EXPECT_NO_THROW(subSubTable->set("Ten", 11.0f));
        EXPECT_NO_THROW(subSubTable->set("AString", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));
        EXPECT_NO_THROW(subTable->setTable("Twelve", *subSubTable));
    }

    // Test value reads
    {
        float oneMapping = -1;
        EXPECT_NO_THROW(table->get("One", oneMapping));
        EXPECT_EQ(2.0f, oneMapping);

        float threeMapping = -1;
        EXPECT_NO_THROW(table->get("Three", threeMapping));
        EXPECT_EQ(4.14f, threeMapping);

        // Grabbing a subtable
        EasyLua::Table retrievedTable;
        EXPECT_NO_THROW(table->get("Five", retrievedTable));

        int sixMapping = -1;
        EXPECT_NO_THROW(retrievedTable.get("Six", sixMapping));
        EXPECT_EQ(7, sixMapping);
    }

    // Make a call with the table
    EasyLua::call(lua, "easyLuaHLTable", 3.14f, table);

    bool result = false;
    EasyLua::Utilities::readStack<false>(lua, &result);

    EXPECT_TRUE(result);

    lua_close(lua);
}
