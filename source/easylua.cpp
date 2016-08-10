#include <easylua.hpp>

namespace EasyLua
{
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
}
