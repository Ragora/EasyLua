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

    lua_getglobal(lua, "easyLuaSingleParamSingleReturn");

    // Pushes a table: {One = 2, Three = 4, Five = { Six = 7, Eight = { Nine = 10 }}}
    EasyLua::Utilities::pushTable(lua, "One", 2,
                                  "Three", 4,
                                  "Five", EasyLua::Utilities::Table(lua, "Six", 7,
                                  "Eight", EasyLua::Utilities::Table(lua, "Nine", 10)));
    lua_call(lua, 1, 1);

    // Deinit
    lua_close(lua);
    std::cout << "Deinitialized Lua" << std::endl;

    return 0;
}
