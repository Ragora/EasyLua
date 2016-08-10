/**
 *  @file easylua.hpp
 *  @author Robert MacGregor
 *  @brief The main include file for the EasyLua library.
 *  @date Sunday, March 15th, 2015
 *  @copyright This software is licensed under the MIT license. Refer to LICENSE.txt
 *  at the root of any source distribution for more information.
 */

#ifndef _INCLUDE_EASYLUA_HPP_
#define _INCLUDE_EASYLUA_HPP_

#include <iostream>
#include <stdexcept>
#include <tuple>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unordered_map>
#include <type_traits>

#include <lua5.2/lua.hpp>

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

    //! Enumeration representing the supported types in the EasyLua library.
    enum EASYLUA_TYPE
    {
        //! Anything that is an integer.
        EASYLUA_INTEGER = 0,
        //! String types.
        EASYLUA_STRING = 1,
        //! Table types.
        EASYLUA_TABLE = 2,
        //! Float types.
        EASYLUA_FLOAT = 3,
    };

    class Table;

    /**
     *  @brief Namespace that contains all of the compile-time resolving code for EasyLua.
     *  The end programmer will not have to interface with anything here directly.
     */
    namespace Resolvers
    {
        /**
         *  @brief The TypeIDResolver template struct is used to resolve EasyLua types to
         *  their internal EasyLua type identification numbers for use in the high level
         *  Table class.
         */
        template <typename type>
        struct TypeIDResolver { };

        /**
         *  @brief The TypeIDResolver template struct is used to resolve EasyLua types to
         *  their internal EasyLua type identification numbers for use in the high level
         *  Table class.
         *  @note This resolver worries about the int type specifically.
         */
        template <>
        struct TypeIDResolver<int>
        {
            static constexpr unsigned char value = EasyLua::EASYLUA_INTEGER;
        };

        /**
         *  @brief The TypeIDResolver template struct is used to resolve EasyLua types to
         *  their internal EasyLua type identification numbers for use in the high level
         *  Table class.
         *  @note This resolver worries about the float type specifically.
         */
        template <>
        struct TypeIDResolver<float>
        {
            static constexpr unsigned char value = EasyLua::EASYLUA_FLOAT;
        };

        /**
         *  @brief The TypeIDResolver template struct is used to resolve EasyLua types to
         *  their internal EasyLua type identification numbers for use in the high level
         *  Table class.
         *  @note This resolver worries about the char type specifically.
         */
        template <>
        struct TypeIDResolver<std::string>
        {
            static constexpr unsigned char value = EasyLua::EASYLUA_STRING;
        };

        template <>
        struct TypeIDResolver<const char*>
        {
            static constexpr unsigned char value = EasyLua::EASYLUA_STRING;
        };

        /**
         *  @brief The TypeIDResolver template struct is used to resolve EasyLua types to
         *  their internal EasyLua type identification numbers for use in the high level
         *  Table class.
         *  @note This resolver worries about the Table type specifically.
         */
        template <>
        struct TypeIDResolver<Table>
        {
            static constexpr unsigned char value = EasyLua::EASYLUA_TABLE;
        };

        /**
         *  @brief The TypeIDResolver template struct is used to resolve EasyLua types to
         *  their internal EasyLua type identification numbers for use in the high level
         *  Table class.
         *  @note This resolver worries about the size_t type specifically.
         */
        template <>
        struct TypeIDResolver<size_t>
        {
            static constexpr unsigned char value = EasyLua::EASYLUA_INTEGER;
        };

        // Table builder
        template <bool createTable>
        struct TableCreationResolver { };

        template <>
        struct TableCreationResolver<true> { static INLINE void resolve(lua_State *lua) { lua_createtable(lua, 0, 0); } };

        template <>
        struct TableCreationResolver<false> { static INLINE void resolve(lua_State *lua) { } };

        // Type Checker
        template <bool typeCheck, typename inputType>
        struct StackReadResolver { };

        static const char *EXCEPTION_FORMAT = "Expected %s (type ID %u) at stack index %u! Got type ID %u instead.";

        template <>
        struct StackReadResolver<true, int>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, int* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TNUMBER)
                {
                    char error[256];
                    sprintf(error, EXCEPTION_FORMAT, "integer (number) ", LUA_TNUMBER, index, type);

                    throw std::runtime_error(error);
                }

                *out = lua_tointeger(lua, index);
                return true;
            }
        };

        template <>
        struct StackReadResolver<true, float>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, float* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TNUMBER)
                {
                    char error[256];
                    sprintf(error, EXCEPTION_FORMAT, "float (number)", LUA_TNUMBER, index, type);

                    throw std::runtime_error(error);
                }

                *out = lua_tonumber(lua, index);
                return true;
            }
        };

        template <>
        struct StackReadResolver<true, char*>
        {
            static INLINE bool resolve(lua_State *lua, const int& index, char* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TSTRING)
                {
                    char error[256];
                    sprintf(error, EXCEPTION_FORMAT, "string", LUA_TSTRING, index, type);

                    throw std::runtime_error(error);
                }

                out = const_cast<char*>(lua_tostring(lua, index));
                return true;
            }
        };

        template <>
        struct StackReadResolver<true, int*>
        {
            static INLINE bool resolve(lua_State *lua, const int& index, void* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TUSERDATA)
                {
                    char error[256];
                    sprintf(error, EXCEPTION_FORMAT, "user data", LUA_TUSERDATA, index, type);

                    throw std::runtime_error(error);
                }

                out = const_cast<void*>(lua_touserdata(lua, index));
                return true;
            }
        };

        // No exception
        template <>
        struct StackReadResolver<false, char*>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, char* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TSTRING)
                    return false;

                out = const_cast<char *>(lua_tostring(lua, index));
                return true;
            }
        };

        template <>
        struct StackReadResolver<false, float>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, float* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TNUMBER)
                    return false;

                *out = lua_tonumber(lua, index);
                return true;
            }
        };

        template <>
        struct StackReadResolver<false, int*>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, void* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TUSERDATA)
                    return false;

                out = lua_touserdata(lua, index);
                return true;
            }
        };

        template <>
        struct StackReadResolver<false, int>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, int* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TNUMBER)
                    return false;

                *out = lua_tointeger(lua, index);
                return true;
            }
        };

        template <>
        struct StackReadResolver<false, bool>
        {
            static INLINE bool resolve(lua_State* lua, const int& index, bool* out)
            {
                const int type = lua_type(lua, index);

                if (type != LUA_TBOOLEAN)
                    return false;

                *out = lua_toboolean(lua, index);
                return true;
            }
        };
    }

    /**
     *  @brief A class that represents Lua table objects. This can be used to read table
     *  returns from the Lua runtime and it may also be used to pass table parameters to
     *  method calls.
     *  @note When using this for passing table parameters, please consider the performance
     *  and memory requirements. So be sure to see if the more lightweight EasyLua::Utilities::Table
     *  is applicable.
     */
    class Table
    {
        // Private Members
        private:
            //! An unordered mapping of keys to stored tables.
            std::unordered_map<std::string, Table*> mTables;

            //! An unordered mapping of keys to arbitrary stored data.
            std::unordered_map<std::string, void*> mContents;

            //! An unordering mapping of keys to type data.
            std::unordered_map<std::string, std::pair<std::string, unsigned char>> mTypes;

        // Public Methods
        public:
            //! Standard destructor.
            ~Table(void)
            {
                // We retain type information here to ensure that the destruction works correctly
                for (auto it = mTypes.begin(); it != mTypes.end(); it++)
                {
                    const std::string name = (*it).first;
                    auto current = (*it).second;

                    switch (current.second)
                    {
                        case EasyLua::EASYLUA_FLOAT:
                        {
                            delete reinterpret_cast<float*>(mContents[name]);
                            break;
                        }

                        case EasyLua::EASYLUA_STRING:
                        {
                            delete reinterpret_cast<std::string*>(mContents[name]);
                            break;
                        }

                        case EasyLua::EASYLUA_INTEGER:
                        {
                            delete reinterpret_cast<int*>(mContents[name]);
                            break;
                        }

                        case EasyLua::EASYLUA_TABLE:
                        {
                            // FIXME: Memory issues here
                            break;
                        }
                    }
                }
            }

            void copy(const Table& other)
            {
                mContents = other.mContents;
                mTypes = other.mTypes;
            }

            template <typename outType>
            void get(const std::string& key, outType& out)
            {
                constexpr unsigned char type = EasyLua::Resolvers::TypeIDResolver<outType>::value;

                if (mTypes.find(key) == mTypes.end())
                    throw std::runtime_error("No such key!");
                else if (mTypes[key].second != type)
                    throw std::runtime_error("Mismatched types!");

                out = *((outType*)(mContents[key]));
            }

            void push(lua_State* lua)
            {
                lua_createtable(lua, 0, 0);

                for (auto it = mTypes.begin(); it != mTypes.end(); it++)
                {
                    const std::string name = (*it).first;
                    auto current = (*it).second;

                    lua_pushstring(lua, current.first.data());

                    switch (current.second)
                    {
                        case EasyLua::EASYLUA_FLOAT:
                        {
                            lua_pushnumber(lua, *reinterpret_cast<float*>(mContents[name]));
                            break;
                        }

                        case EasyLua::EASYLUA_STRING:
                        {
                            lua_pushstring(lua, reinterpret_cast<std::string*>(mContents[name])->data());
                            break;
                        }

                        case EasyLua::EASYLUA_INTEGER:
                        {
                            lua_pushinteger(lua, *reinterpret_cast<int*>(mContents[name]));
                            break;
                        }

                        case EasyLua::EASYLUA_TABLE:
                        {
                            mTables[name]->push(lua);
                            break;
                        }
                    }

                    lua_settable(lua, -3);
                }
            }

            template <typename storedType>
            void set(std::string key, storedType value)
            {
                storedType* memory = new storedType(value);

                mContents[key] = memory;

                constexpr unsigned char type = Resolvers::TypeIDResolver<storedType>::value;
                mTypes[key] = std::make_pair(key, type);
            }

            void setTable(const std::string& key, Table& value)
            {
                mTables[key] = &value;
                mTypes[key] = std::make_pair(key, EasyLua::EASYLUA_TABLE);
                mContents[key] = &value;
            }
    };


    class HighPerformance
    {

    };

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
            static INLINE void pushParameters(lua_State* lua, const int& in, parameters... params)
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
            static INLINE void pushParameters(lua_State* lua, const float& in, parameters... params)
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
            static INLINE void pushParameters(lua_State* lua, const char* in, parameters... params)
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
            static INLINE void pushParameters(lua_State* lua, const bool& in, parameters... params)
            {
                lua_pushboolean(lua, in);
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            template <typename... parameters>
            static INLINE void pushParameters(lua_State* lua, Table& in, parameters... params)
            {
                in.push(lua);
                EasyLua::Utilities::pushParameters(lua, params...);
            }

            template <typename... parameters>
            static INLINE void pushParameters(lua_State* lua, Table* in, parameters... params)
            {
                in->push(lua);
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
            static INLINE void pushTable(lua_State* lua, const char* key, const int& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

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
            static INLINE void pushTable(lua_State* lua, const char* key, const float& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

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
            static INLINE void pushTable(lua_State* lua, const char* key, const double& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

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
            static INLINE void *pushTable(lua_State* lua, const char* key, const bool& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

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
            static INLINE void pushTable(lua_State* lua, const char* key, const char* value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

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
            static INLINE void pushTableComponents(lua_State* lua, parameters... params)
            {
                EasyLua::Utilities::pushTable<false>(lua, params...);
            }

            /**
             *  @brief A helper method that can be used to push subtables to the Lua
             *  stack when using the high performance interface.
             *  @example subtables/main.cpp
             */
            template <int depth, typename... parameters>
            static INLINE void *Table(lua_State* lua, parameters... params)
            {
                const size_t currentTop = lua_gettop(lua);

                EasyLua::Utilities::pushTable<true>(lua, params...);

                // TODO (Robert MacGregor#9): Statically resolve the necessity of this, it's only necessary for pushParameters
                lua_insert(lua, depth); // Puts pushed tables in the right order for pushParameters

                return nullptr;
            }

            // Array Pusher
            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State* lua, const int& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushinteger(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State* lua, const char* key, const float& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State* lua, const char* key, const double& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushnumber(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State *lua, const char* key, const bool& value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushboolean(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1, typename... parameters>
            static INLINE void pushArray(lua_State* lua, const char* key, const char* value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);

                lua_pushinteger(lua, index);
                lua_pushstring(lua, value);
                lua_settable(lua, -3);

                EasyLua::Utilities::pushArray<false, index + 1>(lua, params...);
            }

            template <bool createTable = true, unsigned int index = 1>
            static INLINE void pushArray(lua_State* lua) { }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE int readStack(lua_State* lua, float* out, parameters... params)
            {
                if (sizeof...(parameters) > lua_gettop(lua))
                    throw std::runtime_error("Not enough values to read (reading float)!");

                if (!EasyLua::Resolvers::StackReadResolver<typeException, float>::resolve(lua, index, out))
                    return index;

                *out = luaL_checknumber(lua, index);
                return EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE int readStack(lua_State* lua, bool* out, parameters... params)
            {
                if (sizeof...(parameters) > lua_gettop(lua))
                    throw std::runtime_error("Not enough values to read (reading bool)!");

                EasyLua::Resolvers::StackReadResolver<typeException, bool>::resolve(lua, index, out);

                *out = lua_toboolean(lua, index);
                return EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE int readStack(lua_State *lua, int* out, parameters... params)
            {
                if (sizeof...(parameters) > lua_gettop(lua))
                    throw std::runtime_error("Not enough values to read (reading int)!");

                if (!EasyLua::Resolvers::StackReadResolver<typeException, int>::resolve(lua, index, out))
                    return index;

                return EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE int readStack(lua_State* lua, char *out, const int &outLength, parameters... params)
            {
                if (sizeof...(parameters) > lua_gettop(lua))
                    throw std::runtime_error("Not enough values to read (reading string)!");

                if (!EasyLua::Resolvers::StackReadResolver<typeException, char *>::resolve(lua, index, out))
                    return index;

                // We want to read a string of max size outLength
                const char *target = luaL_checkstring(lua, index);

                // Copy the target memory to our out pointer
                unsigned int targetLength = strlen(target);
                targetLength = outLength > targetLength ? targetLength : targetLength - outLength;
                memcpy(out, target, targetLength);

                return EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE int readStack(lua_State* lua, std::string *out, parameters... params)
            {
                if (sizeof...(parameters) > lua_gettop(lua))
                    throw std::runtime_error("Not enough values to read (reading string)!");

                char *outTest = 0x00;

                if (!EasyLua::Resolvers::StackReadResolver<typeException, char *>::resolve(lua, index, outTest))
                    return index;

                out->assign(luaL_checkstring(lua, index));

                return EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            template <bool typeException, int index = 1, typename... parameters>
            static INLINE int readStack(lua_State* lua, void* out, parameters... params)
            {
                if (sizeof...(parameters) > lua_gettop(lua))
                    throw std::runtime_error("Not enough values to read (reading userdata)!");

                EasyLua::Resolvers::StackReadResolver<typeException, int *>::resolve(lua, index);

                out = lua_touserdata(lua, index);
                return EasyLua::Utilities::readStack<typeException, index + 1>(lua, params...);
            }

            static void printStack(lua_State* lua)
            {
                std::cout << "Lua Stack Dump ------ " << std::endl;

                for (int iteration = 0; iteration < lua_gettop(lua) + 1; iteration++)
                {
                    const int currentType = lua_type(lua, iteration);
                    std::cout << iteration << ": ";

                    switch (currentType)
                    {
                        case LUA_TNUMBER:
                        {
                            std::cout << "NUMBER (" << LUA_TNUMBER << ") = " << luaL_checknumber(lua, iteration);
                            break;
                        }

                        case LUA_TSTRING:
                        {
                            std::cout << "STRING (" << LUA_TSTRING << ") = '" << luaL_checkstring(lua, iteration) << "'";
                            break;
                        }

                        case LUA_TFUNCTION:
                        {
                            std::cout << "FUNCTION (" << LUA_TFUNCTION << ") ";
                            break;
                        }

                        case LUA_TTABLE:
                        {
                            std::cout << "TABLE (" << LUA_TTABLE << ") ";
                            break;
                        }

                        case LUA_TBOOLEAN:
                        {
                            std::cout << "BOOLEAN (" << LUA_TBOOLEAN << ") = " << (lua_toboolean(lua, iteration) ? "true" : "false");
                            break;
                        }

                        default:
                        {
                            std::cout << "UNKNOWN TYPE (" << currentType << ") ";
                            break;
                        }
                    }

                    std::cout << std::endl;
                }
            }

            /**
             *  @brief This is one among a family of methods that push arbitrary values to the
             *  Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @param in This value is actually not used.
             *  @param params The rest of the parameters to be pushing to the Lua stack.
             *  @note The end programmer should not be using this method directly.
             */
            template <typename... parameters>
            static INLINE void pushParameters(lua_State* lua, const void* value, parameters... params)
            {
                // Tables will stack at the bottom so we correct this as we run
                lua_pushvalue(lua, 1);
                lua_remove(lua, 1);

                EasyLua::Utilities::pushParameters(lua, params...);
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
            static INLINE void pushParameters(lua_State* lua) { }

            /**
             *  @brief This is one among a family of methods that push an array containing
             *  arbitrary values to the Lua stack.
             *  @param lua A pointer to the lua_State to use for this operation.
             *  @note The end programmer should not be using this method directly.
             */
            template <bool createTable = true>
            static INLINE void pushTable(lua_State* lua) { }

            template <bool createTable = true, typename... parameters>
            static INLINE void pushTable(lua_State* lua, const char* key, const void* value, parameters... params)
            {
                EasyLua::Resolvers::TableCreationResolver<createTable>::resolve(lua);
                EasyLua::Utilities::pushTable<false>(lua, params...);

                // We'll have two tables at the top and we need to swap them for correct assignment as
                // the ordering will be inner then outer
                lua_insert(lua, -2);
                lua_pushstring(lua, key);
                lua_insert(lua, -2);
                lua_settable(lua, -3);
            }

            template <bool typeException, int index = -1>
            static INLINE int readStack(lua_State* lua) { return -1; }
    }; // End Class Utilities

    /**
     *  @brief Performs an unprotected Lua call.
     *  @param lua A pointer to the lua_State to perform this operation against.
     *  @param methodName A string representing the name of the global method in the Lua runtime to call.
     *  @example methodcalls/main.cpp
     */
    template <typename... parameters>
    static INLINE unsigned int call(lua_State* lua, const char* methodName, parameters... params)
    {
        const int oldTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        lua_call(lua, sizeof...(params), LUA_MULTRET);

        return abs(lua_gettop(lua) - oldTop);
    }

    static INLINE unsigned int call(lua_State* lua, const char* methodName, const EasyLua::ParameterCount& parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        lua_insert(lua, 1);

        lua_call(lua, parameterCount, LUA_MULTRET);

        return lua_gettop(lua) - stackTop;
    }

    template <typename... parameters>
    static INLINE std::pair<int, size_t> pcall(lua_State* lua, const char* methodName, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        return std::make_pair(lua_pcall(lua, sizeof...(params), LUA_MULTRET, 0), lua_gettop(lua) - stackTop);
    }

    static INLINE std::pair<int, size_t> pcall(lua_State* lua, const char* methodName, const EasyLua::ParameterCount& parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, methodName);
        return std::make_pair(lua_pcall(lua, parameterCount, LUA_MULTRET, 0), lua_gettop(lua) - stackTop);
    }

    static INLINE std::pair<int, size_t> pcall(lua_State* lua, const char* methodName, const char* errorHandler, const EasyLua::ParameterCount& parameterCount)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, errorHandler);
        lua_getglobal(lua, methodName);

        return std::make_pair(lua_pcall(lua, parameterCount, LUA_MULTRET, -2), lua_gettop(lua) - stackTop);
    }

    template <typename... parameters>
    static INLINE std::pair<int, size_t> pcall(lua_State* lua, const char* methodName, const char* errorHandler, parameters... params)
    {
        const int stackTop = lua_gettop(lua);

        lua_getglobal(lua, errorHandler);
        lua_getglobal(lua, methodName);
        EasyLua::Utilities::pushParameters(lua, params...);

        return std::make_pair(lua_pcall(lua, sizeof...(params), LUA_MULTRET, sizeof...(params) - 2), lua_gettop(lua) - stackTop);
    } // End "NameSpace" Utilities
} // End NameSpace EasyLua

#endif // _INCLUDE_EASYLUA_HPP_
