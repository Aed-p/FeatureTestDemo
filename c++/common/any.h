#pragma once

#include "base.h"
#include <memory>

namespace FEATURE_TEST
{

class Any
{
public:
    Any(/* args */) = default;
    ~Any() = default;
    Any(const Any &other);
    Any(Any &&other);
    Any &operator=(const Any &other);
    Any &operator=(Any &&other);

    template <typename T>
    Any(const T &value)
        : data_ptr(std::make_shared<T>(value))
    {
    }

    template <typename T>
    Any(T &&value)
        : data_ptr(std::make_shared<T>(std::move(value)))
    {
    }

    template <typename T>
    T &cast()
    {
        return *std::static_pointer_cast<T>(data_ptr);
    }

    template <typename T>
    const T &cast() const
    {
        return *std::static_pointer_cast<T>(data_ptr);
    }

    template <typename T>
    operator T &()
    {
        return cast<T>();
    }

    template <typename T>
    operator const T &() const
    {
        return cast<T>();
    }

private:
    /* data */
    std::shared_ptr<void> data_ptr;

};

} // namespace FEATURE_TEST
