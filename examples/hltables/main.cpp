/**
 *  @file main.cpp
 *  @brief Example coding showing the usage of the high level
 *  tables interface.
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt
 *  at the root of any source distribution for more information.
 */

#include <iostream>

#include <easylua.hpp>

int main(int argc, char *argv[])
{
    // Init Lua
    lua_State *lua = luaL_newstate();
    luaL_checkversion(lua);
    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, 0);
    luaL_dofile(lua, "main.lua");

    // Init and setup our HL table
    EasyLua::Table table;
    table.set("One", 2);
    table.set("Three", 4.14f);

    EasyLua::Table subTable;
    subTable.set("Six", 7);
    subTable.set("Eight", 9.14f);

    table.setTable("Five", subTable);

    // You can read these values
    try
    {
        size_t oneMapping = -1;
        table.get("One", oneMapping);

        float threeMapping = -1;
        table.get("Three", threeMapping);

        std::cout << "One Mapping: " << oneMapping << std::endl;
        std::cout << "Three Mapping: " << threeMapping << std::endl;

        subTable = EasyLua::Table();
        table.get("Five", subTable);

        int sixMapping = -1;
        subTable.get("Six", sixMapping);
        std::cout << "Six Mapping: " << sixMapping << std::endl;
    }
    catch (std::runtime_error &e)
    {
        std::cout << "Caught Exception: " << e.what() << std::endl;
    }

    // Push the table to the stack
    table.push(lua);

    EasyLua::Utilities::printStack(lua);

  //  EasyLua::Utilities::printStack(lua);

    const int returnCount = EasyLua::call(lua, "easyLuaHLTable", EasyLua::ParameterCount(1));

    // Deinit
    lua_close(lua);
    std::cout << "---- Deinitialized Lua" << std::endl;

    return 0;
}
