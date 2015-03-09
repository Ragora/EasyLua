/**
 *  @file easylua.hpp
 *  @author Robert MacGregor
 *  @brief The main include file for the EasyLua library.
 *  @date Monday, March 2nd, 2015
 *  @copyright This software is licensed under the MIT license. Please refer
 *  to LICENSE.txt for more information.
 */

#include <stdexcept>
#include <cstdlib>
#include <cstring>

#include <lua.hpp>

// Define __forceinline if we're on GCC
#if defined(__GNUC__) || defined(__GNUG__)
    #define __forceinline __attribute__((always_inline))
#endif

#define INLINE __forceinline

/**
 *  @brief This namespace contains all of the EasyLua API which is to prevent accidental
 *  naming conflicts.
 */
namespace EasyLua
{
    typedef size_t StringLength;
    typedef size_t ParameterCount;
    typedef size_t ReturnCount;

    // Table builder
    template <bool createTable>
    struct TableCreationResolver { };

    template <>
    struct TableCreationResolver<true> { static INLINE void resolve(lua_State *lua) { lua_createtable(lua, 0, 0); } };

    template <>
    struct TableCreationResolver<false> { static INLINE void resolve(lua_State *lua) { } };

    // Type Checker
    template <bool typeCheck, typename inputType>
    struct TypeExceptionResolver { };

    static const char *EXCEPTION_FORMAT = "Expected %s (type ID %u) at stack index %u! Got type ID %u instead.";

    template <>
    struct TypeExceptionResolver<true, int>
    {
        static INLINE void resolve(lua_State *lua, const int &index)
        {
            const int type = lua_type(lua, index);

            if (lua_type(lua, index) != LUA_TNUMBER)
            {
                char error[256];
                sprintf(error, EXCEPTION_FORMAT, "integer (number) ", LUA_TNUMBER, index, type);

                throw std::runtime_error(error);
            }
        }
    };

    template <>
    struct TypeExceptionResolver<true, float>
    {
        static INLINE void resolve(lua_State *lua, const int &index)
        {
            const int type = lua_type(lua, index);

            if (lua_type(lua, index) != LUA_TNUMBER)
            {
                char error[256];
                sprintf(error, EXCEPTION_FORMAT, "float (number)", LUA_TNUMBER, index, type);

                throw std::runtime_error(error);
            }
        }
    };

    template <>
    struct TypeExceptionResolver<true, char *>
    {
        static INLINE void resolve(lua_State *lua, const int &index)
        {
            const int type = lua_type(lua, index);

            if (lua_type(lua, index) != LUA_TSTRING)
            {
                char error[256];
                sprintf(error, EXCEPTION_FORMAT, "string", LUA_TSTRING, index, type);

                throw std::runtime_error(error);
            }
        }
    };

    template <>
    struct TypeExceptionResolver<true, int *>
    {
        static INLINE void resolve(lua_State *lua, const int &index)
        {
            const int type = lua_type(lua, index);

            if (lua_type(lua, index) != LUA_TUSERDATA)
            {
                char error[256];
                sprintf(error, EXCEPTION_FORMAT, "user data", LUA_TUSERDATA, index, type);

                throw std::runtime_error(error);
            }
        }
    };

    template <>
    struct TypeExceptionResolver<false, char *> { static INLINE void resolve(lua_State *lua, const int &index) { } };
    template <>
    struct TypeExceptionResolver<false, float> { static INLINE void resolve(lua_State *lua, const int &index) { } };
    template <>
    struct TypeExceptionResolver<false, int *> { static INLINE void resolve(lua_State *lua, const int &index) { } };
    template <>
    struct TypeExceptionResolver<false, int> { static INLINE void resolve(lua_State *lua, const int &index) { } };

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
            static INLINE void pushParameters(lua_State *lua, const int &in, parameters... params)
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
            static INLINE void pushParameters(lua_State *lua, const float &in, parameters... params)
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
            static INLINE void pushParameters(lua_State *lua, const char *in, parameters... params)
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
            static INLINE void pushParameters(lua_State *lua, const bool &in, parameters... params)
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
            static INLINE void pushTable(lua_State *lua, const char *key, const int &value, parameters... params)
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
            static INLINE void pushTable(lua_State *lua, const char *key, const float &value, parameters... params)
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
            static INLINE void pushTable(lua_State *lua, const char *key, const double &value, parameters... params)
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
            static INLINE void *pushTable(lua_State *lua, const char *key, const bool &value, parameters... params)
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
            static INLINE void pushTable(lua_State *lua, const char *key, const char *value, parameters... params)
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
            static INLINE void pushTableComponents(lua_State *lua, parameters... params)
            {
                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief A helper method that can be used to push subtables to the Lua
             *  stack.
             *  @example subtables/main.cpp
             */
            template <typename... parameters>
            static INLINE void *Table(lua_State *lua, parameters... params)
            {
                EasyLua::Utilities::pushTable<true>(lua, params...);
                return NULL;
            }

            // Array Pusher
            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State *lua, const int &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushinteger(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State *lua, const char *key, const float &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State *lua, const char *key, const double &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State *lua, const char *key, const bool &value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushboolean(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State *lua, const char *key, const char *value, parameters... params)
            {
                EasyLua::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushstring(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1>
            static INLINE void pushArray(lua_State *lua) { }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE void readStack(lua_State *lua, float *out, parameters... params)
            {
                EasyLua::TypeExceptionResolver<typeException, float>::resolve(lua, index);

                *out = luaL_checknumber(lua, index);
                EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE void readStack(lua_State *lua, bool *out, parameters... params)
            {
                EasyLua::TypeExceptionResolver<typeException, int>::resolve(lua, index);

                *out = luaL_checkinteger(lua, index);
                EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE void readStack(lua_State *lua, int *out, parameters... params)
            {
                EasyLua::TypeExceptionResolver<typeException, int>::resolve(lua, index);

                 *out = luaL_checkinteger(lua, index);
                EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE void readStack(lua_State *lua, char *out, const int &outLength, parameters... params)
            {
                EasyLua::TypeExceptionResolver<typeException, char *>::resolve(lua, index);

                // We want to read a string of max size outLength
                const char *target = luaL_checkstring(lua, index);

                // Copy the target memory to our out pointer
                unsigned int targetLength = strlen(target);
                targetLength = outLength > targetLength ? targetLength : targetLength - outLength;
                memcpy(out, target, targetLength);

                EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE void readStack(lua_State *lua, std::string *out, parameters... params)
            {
                EasyLua::TypeExceptionResolver<typeException, char *>::resolve(lua, index);

                out->assign(luaL_checkstring(lua, index));

                EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }


            template <bool typeException, int index = 1, typename... parameters>
            static INLINE void readStack(lua_State *lua, void *out, parameters... params)
            {
                EasyLua::TypeExceptionResolver<typeException, int *>::resolve(lua, index);

                out = lua_touserdata(lua, index);
                EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
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
            static INLINE void pushParameters(lua_State *lua) { }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in This value is actually not used.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             *  @note The end programmer should not be using this method directly.
             */
            template <typename... parameters>
            static INLINE void pushParameters(lua_State *lua, const void *in, parameters... params)
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
            static INLINE void pushTable(lua_State *lua) { }

            template <bool createTable = true, typename... parameters>
            static INLINE void pushTable(lua_State *lua, const char *key, const void *value, parameters... params)
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

            template <bool typeException, int index = -1>
            static INLINE void readStack(lua_State *lua) { }
    }; // End Class Utilities

    /**
     *  @brief Performs an unprotected Lua call.
     *  @param lua A pointer to the lua_State to perform this operation against.
     *  @param methodName A string representing the name of the global method in the Lua runtime to call.
     *  @example methodcalls/main.cpp
     */
    template <typename... parameters>
    static INLINE unsigned int call(lua_State *lua, const char *methodName, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);
        lua_call(lua, sizeof...(params), LUA_MULTRET);

        return lua_gettop(lua) - stackTop;
    }

   // static INLINE unsigned int call(lua_State *lua, const char *methodName, const EasyLua::ParameterCount &parameterCount)
   // {
   //     const int stackTop = lua_gettop(lua);

     //   lua_getglobal(lua, methodName);
      //  lua_call(lua, parameterCount, LUA_MULTRET);

       // return lua_gettop(lua) - stackTop;
   // }

    template <typename... parameters>
    static INLINE std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        return std::make_pair(lua_pcall(lua, sizeof...(params), LUA_MULTRET, 0), lua_gettop(lua) - stackTop);
    }

    static INLINE std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, const EasyLua::ParameterCount &parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        return std::make_pair(lua_pcall(lua, parameterCount, LUA_MULTRET, 0), lua_gettop(lua) - stackTop);
    }

    static INLINE std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, const char *errorHandler, const EasyLua::ParameterCount &parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, errorHandler);
        lua_getglobal(lua, methodName);

        return std::make_pair(lua_pcall(lua, parameterCount, LUA_MULTRET, -2), lua_gettop(lua) - stackTop);
    }

    template <typename... parameters>
    static INLINE std::pair<int, size_t> pcall(lua_State *lua, const char *methodName, const char *errorHandler, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, errorHandler);
        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        return std::make_pair(lua_pcall(lua, sizeof...(params), LUA_MULTRET, sizeof...(params) - 2), lua_gettop(lua) - stackTop);
    } // End "NameSpace" Utilities
} // End NameSpace EasyLua
