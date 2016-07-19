/**
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

    std::cout << "Initialized Lua " << std::endl;

    /*
        The call below performs the following Lua call:
        tableOne = { Test = 3 }
        tableTwo = { Six = 7, Eight = { Nine = 10 }}
        tableThree = { Ten = 11 }
        tableFour = { Thirteen = 14 }
        easyLuaMultiTables(tableOne, "One", 2, "Three", 4.12, "Five", tableTwo, tableThree, 12, tableFour)
    */
    EasyLua::call(lua, "easyLuaMultiTables",
                                  EasyLua::Utilities::Table<1>(lua, "Test", 3),
                                  "One", 2, "Three", 4.12f, "Five",
                                  EasyLua::Utilities::Table<1>(lua, "Six", 7, "Eight", EasyLua::Utilities::Table<0>(lua, "Nine", 10, "Another", EasyLua::Utilities::Table<-1>(lua, "Table", 50))),
                                  EasyLua::Utilities::Table<1>(lua, "Ten", 11),
                                  12,
                                  EasyLua::Utilities::Table<1>(lua, "Thirteen", 14));

    // Deinit
    lua_close(lua);
    std::cout << "Deinitialized Lua" << std::endl;

    return 0;
}
