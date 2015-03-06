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

    const unsigned int returnCount = EasyLua::call(lua, "easyLuaMultiParamMultiReturnTest", 1, "Two", 3.14f);

    std::cout << "Return Count: " << returnCount << std::endl;
    int integerReturn = -1;
    float floatReturn = -1;
    char stringReturn[256];

    std::cout << sizeof(stringReturn) << std::endl;
    EasyLua::Utilities::readStack(lua, &integerReturn, &floatReturn, stringReturn);

    std::cout << "Got Int: " << integerReturn << std::endl;
    std::cout << "Got Float: " << floatReturn << std::endl;
    std::cout << "Got String: " << stringReturn << std::endl;

    // Deinit
    lua_close(lua);
    std::cout << "Deinitialized Lua" << std::endl;

    return 0;
}
