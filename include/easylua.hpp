/**
 *  @file easylua.hpp
 *  @author Robert MacGregor
 *  @brief The main include file for the EasyLua library.
 *  @date Monday, March 2nd, 2015
 *  @copyright This software is licensed under the MIT license. Please refer
 *  to LICENSE.txt for more information.
 */

#include <cstdlib>
#include <cstring>

#include <lua.hpp>

/**
 *  @brief This namespace contains all of the EasyLua API which is to prevent accidental
 *  naming conflicts.
 */
namespace EasyLua
{
    typedef size_t StringLength;
    typedef size_t ParameterCount;
    typedef size_t ReturnCount;

    template <bool createTable>
    struct TableCreationResolver { };

    template <>
    struct TableCreationResolver<true> { static inline void resolve(lua_State *lua) { lua_createtable(lua, 0, 0); } };

    template <>
    struct TableCreationResolver<false> { static inline void resolve(lua_State *lua) { } };

    /**
     *  @brief This "namespace" contains a bulk of the EasyLua API that the end programmer
     *  should be concerned with.
     *  @note This is actually a class with static methods because of the usage of recursive
     *  templates to resolve the Lua API calls.
     */
    class Utilities
    {
        // Public Methods
        public:

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in The current int to be pushing to the Lua stack.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             */
            template <typename... parameters>
            static inline void pushParameters(lua_State *lua, const int &in, parameters... params)
            {
                lua_pushinteger(lua, in);
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in The current float to be pushing to the Lua stack.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             */
            template <typename... parameters>
            static inline void pushParameters(lua_State *lua, const float &in, parameters... params)
            {
                lua_pushnumber(lua, in);
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in The current string to be pushing to the Lua stack.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             */
            template <typename... parameters>
            static inline void pushParameters(lua_State *lua, const char *in, parameters... params)
            {
                lua_pushstring(lua, in);
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in The current bool to be pushing to the Lua stack.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             */
            template <typename... parameters>
            static inline void pushParameters(lua_State *lua, const bool &in, parameters... params)
            {
                lua_pushboolean(lua, in);
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push a table containing
             *  arbitrary values to the Lua stack.
             *  @param key The key to assign the value to so that { key = value }
             *  @param value The int value to assign to our key in the table.
             *  @param lua A pointer to the lua_State to use for this operation.
             */
            template <bool createTable = true, typename... parameters>
            static inline void pushTable(lua_State *lua, const char *key, const int &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushstring(lua, key);
                lua_pushinteger(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push a table containing
             *  arbitrary values to the Lua stack.
             *  @param key The key to assign the value to so that { key = value }
             *  @param value The float value to assign to our key in the table.
             *  @param lua A pointer to the lua_State to use for this operation.
             */
            template <bool createTable = true, typename... parameters>
            static inline void pushTable(lua_State *lua, const char *key, const float &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushstring(lua, key);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push a table containing
             *  arbitrary values to the Lua stack.
             *  @param key The key to assign the value to so that { key = value }
             *  @param value The double value to assign to our key in the table.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @note The double is probably downcasted to a float for this operation.
             */
            template <bool createTable = true, typename... parameters>
            static inline void pushTable(lua_State *lua, const char *key, const double &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushstring(lua, key);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push a table containing
             *  arbitrary values to the Lua stack.
             *  @param key The key to assign the value to so that { key = value }
             *  @param value The bool value to assign to our key in the table.
             *  @param lua A pointer to the lua_State to use for this operation.
             */
            template <bool createTable = true, typename... parameters>
            static inline void *pushTable(lua_State *lua, const char *key, const bool &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushstring(lua, key);
                lua_pushboolean(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push a table containing
             *  arbitrary values to the Lua stack.
             *  @param key The key to assign the value to so that { key = value }
             *  @param value The string value to assign to our key in the table.
             *  @param lua A pointer to the lua_State to use for this operation.
             */
            template <bool createTable = true, typename... parameters>
            static inline void pushTable(lua_State *lua, const char *key, const char *value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushstring(lua, key);
                lua_pushstring(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief A helper method that can be used to push more components to the
             *  current table.
             *  @param key The key to assign the value to so that { key = value }
             *  @param value The string value to assign to our key in the table.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @note This simply calls EasyLua::Utilities::pushTable<false>(lua, params...)
             */
            template <typename... parameters>
            static inline void pushTableComponents(lua_State *lua, parameters... params)
            {
                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief A helper method that can be used to push subtables to the Lua
             *  stack.
             *  @example subtables/main.cpp
             */
            template <typename... parameters>
            static inline void *Table(lua_State *lua, parameters... params)
            {
                EasyLua::Utilities::pushTable<true>(lua, params...);
                return NULL;
            }

            // Array Pusher
            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static inline void pushArray(lua_State *lua, const int &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushinteger(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static inline void pushArray(lua_State *lua, const char *key, const float &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static inline void pushArray(lua_State *lua, const char *key, const double &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static inline void pushArray(lua_State *lua, const char *key, const bool &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushboolean(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static inline void pushArray(lua_State *lua, const char *key, const char *value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushstring(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1>
            static inline void pushArray(lua_State *lua) { }

            template <int index = -1, typename... parameters>
            static inline void readStack(lua_State *lua, float *out, parameters... params)
            {
                *out = luaL_checknumber(lua, -index);
                EasyLua::Utilities::readStack<index - 1>(lua, params...);
            }

            template <int index = -1, typename... parameters>
            static inline void readStack(lua_State *lua, bool *out, parameters... params)
            {
                *out = luaL_checkinteger(lua, -index);
                EasyLua::Utilities::readStack<index - 1>(lua, params...);
            }

            template <int index = -1, typename... parameters>
            static inline void readStack(lua_State *lua, int *out, parameters... params)
            {
                *out = luaL_checkinteger(lua, -index);
                EasyLua::Utilities::readStack<index - 1>(lua, params...);
            }

            template <int index = -1, typename... parameters>
            static inline void readStack(lua_State *lua, char out[], parameters... params)
            {
                // We want to read a string of max size outLength
                const char *target = luaL_checkstring(lua, index);
                const unsigned int targetLength = strlen(target);

                std::cout << index << std::endl;
                //if (outLength > targetLength)
                    memcpy(out, target, targetLength);
               // else
                //    memcpy(out, target, outLength);

                EasyLua::Utilities::readStack<index - 1>(lua, params...);
            }

            template <int index = -1, typename... parameters>
            static inline void readStack(lua_State *lua, char *out, const EasyLua::StringLength &stringLength, parameters... params)
            {
                // We want to read a string of max size outLength
                const char *target = luaL_checkstring(lua, index);
                const unsigned int targetLength = strlen(target);

                std::cout << index << std::endl;
                //if (outLength > targetLength)
                    memcpy(out, target, targetLength);
               // else
                //    memcpy(out, target, outLength);

                EasyLua::Utilities::readStack<index - 1>(lua, params...);
            }

            template <int index = -1, typename... parameters>
            static inline void readStack(lua_State *lua, void *out, parameters... params)
            {
                out = lua_touserdata(lua, index);
                EasyLua::Utilities::readStack<index - 1>(lua, params...);
            }

        // Private Methods
        private:
            //! Private constructor.
            Utilities(void) { }
            //! Private destructor.
            ~Utilities(void) { }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @note The end programmer should not be using this method directly.
             */
            static inline void pushParameters(lua_State *lua) { }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in This value is actually not used.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             *  @note The end programmer should not be using this method directly.
             */
            template <typename... parameters>
            static inline void pushParameters(lua_State *lua, const void *in, parameters... params)
            {
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            /**
             *  @brief This is one among a family of methods that push an array containing
             *  arbitrary values to the Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @note The end programmer should not be using this method directly.
             */
            template <bool createTable = true>
            static inline void pushTable(lua_State *lua) { }

            template <bool createTable = true, typename... parameters>
            static inline void pushTable(lua_State *lua, const char *key, const void *value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                EasyLua::Utilities::pushTable<false>(lua, params...);

                // We'll have two tables at the top and we need to swap them for correct assignment as
                // the ordering will be inner then outer
                lua_insert(lua, -2);
                lua_pushstring(lua, key);
                lua_insert(lua, -2);
                lua_settable(lua, -3);
            }

            template <int index = -1>
            static inline void readStack(lua_State *lua) { }
    }; // End Class Utilities

    /**
     *  @brief Performs an unprotected Lua call.
     *  @param lua A pointer to the lua_State to perform this operation against.
     *  @param methodName A string representing the name of the global method in the Lua runtime to call.
     *  @example methodcalls/main.cpp
     */
    template <typename... parameters>
    static inline unsigned int call(lua_State *lua, const char *methodName, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);
        lua_call(lua, sizeof...(params), LUA_MULTRET);

        return lua_gettop(lua) - stackTop;
    }

    static inline unsigned int call(lua_State *lua, const char *methodName, const EasyLua::ParameterCount &parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        lua_call(lua, parameterCount, LUA_MULTRET);

        return lua_gettop(lua) - stackTop;
    }

    template <typename... parameters>
    static inline std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        return std::make_pair(lua_pcall(lua, sizeof...(params), LUA_MULTRET, 0), lua_gettop(lua) - stackTop);
    }

    static inline std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, const EasyLua::ParameterCount &parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        return std::make_pair(lua_pcall(lua, parameterCount, LUA_MULTRET, 0), lua_gettop(lua) - stackTop);
    }

    static inline std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, const char *errorHandler, const EasyLua::ParameterCount &parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, errorHandler);
        lua_getglobal(lua, methodName);

        return std::make_pair(lua_pcall(lua, parameterCount, LUA_MULTRET, -2), lua_gettop(lua) - stackTop);
    }

    template <typename... parameters>
    static inline std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, const char *errorHandler, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, errorHandler);
        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        return std::make_pair(lua_pcall(lua, sizeof...(params), LUA_MULTRET, sizeof...(params) - 2), lua_gettop(lua) - stackTop);
    } // End "NameSpace" Utilities
} // End NameSpace EasyLua
