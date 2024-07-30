#pragma once

#include <type_traits>

namespace FEATURE_TEST
{

struct TypeMap
{
    template <typename T>
    static std::type_info & get_type_info()
    {
        static std::type_info type_info = typeid(T);
        return type_info;
    }
};




} // namespace FEATURE_TEST
