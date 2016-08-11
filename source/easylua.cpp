/**
 *  @file easylua.cpp
 *  @brief Source file implementing non-templated logic in EasyLua.
 *
 *  This software is licensed under the MIT license. Refer to LICENSE.txt for
 *  more information.
 *
 *  @date 8/10/2016
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Robert MacGregor
 */

#include <easylua.hpp>

namespace EasyLua
{
    Table::Table(void)
    {
    }

    Table::Table(Table& other)
    {
        this->clear(true);
        this->copy(other);
    }

    Table::~Table(void)
    {
        this->clear(true);
    }

    void Table::clear(bool deleteChildren)
    {
        for (auto it = mTypes.begin(); it != mTypes.end(); it++)
        {
            auto current = *it;

            const std::string& name = current.first;
            const unsigned char type = current.second.second;
            void* deletedMemory = mContents[name];

            switch(type)
            {
                case EASYLUA_FLOAT:
                {
                    delete reinterpret_cast<float*>(deletedMemory);
                    break;
                }

                case EASYLUA_INTEGER:
                {
                    delete reinterpret_cast<int*>(deletedMemory);
                    break;
                }

                case EASYLUA_STRING:
                {
                    delete reinterpret_cast<std::string*>(deletedMemory);
                    break;
                }

                case EASYLUA_TABLE:
                {
                    if (deleteChildren)
                        delete reinterpret_cast<Table*>(deletedMemory);

                    break;
                }
            }
        }
    }

    template <>
    void Table::set(std::string key, char* value)
    {
        this->set(key, std::string(value));
    }

    template <>
    void Table::set(std::string key, const char* value)
    {
        this->set(key, std::string(value));
    }

    template <>
    void Table::get(const std::string& key, Table& out)
    {
        if (mTypes.count(key) == 0)
            throw std::runtime_error("No such key!");
        else if (mTypes[key].second != EasyLua::EASYLUA_TABLE)
            throw std::runtime_error("Mismatched types!");

        Table* target = reinterpret_cast<Table*>(mContents[key]);
        out.copy(*target);
    }

    void Table::copy(Table& other)
    {
        mTypes.clear();
        mContents.clear();

        for (auto it = other.mTypes.begin(); it != other.mTypes.end(); it++)
        {
            auto current = *it;

            const std::string& name = current.first;
            const unsigned char type = current.second.second;
            void* copiedMemory = other.mContents[name];

            void* newMemory = nullptr;

            // Allocate a new block
            switch (type)
            {
                case EASYLUA_FLOAT:
                {
                    float* newFloat = new float(*reinterpret_cast<float*>(copiedMemory));
                    newMemory = newFloat;

                    break;
                }

                case EASYLUA_INTEGER:
                {
                    int* newInt = new int(*reinterpret_cast<int*>(copiedMemory));
                    newMemory = newInt;

                    break;
                }

                case EASYLUA_STRING:
                {
                    std::string* newString = new std::string(*reinterpret_cast<std::string*>(copiedMemory));
                    newMemory = newString;

                    break;
                }

                case EASYLUA_TABLE:
                {
                    Table* newTable = new Table();
                    newTable->copy(*reinterpret_cast<Table*>(copiedMemory));
                    newMemory = newTable;

                    break;
                }
            }

            assert(newMemory);

            mContents[name] = newMemory;
            mTypes[name] = std::make_pair(name, type);
        }
    }

    void Table::push(lua_State* lua)
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

    void Table::setTable(const std::string& key, Table& value)
    {
        mTables[key] = &value;
        mTypes[key] = std::make_pair(key, EasyLua::EASYLUA_TABLE);
        mContents[key] = &value;
    }
}
