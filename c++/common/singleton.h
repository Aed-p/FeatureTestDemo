#pragma once


#include <memory>


namespace TestDemo
{

template <typename T>
class Singleton
{
public:
    /// @brief get the instance of the singleton
    static T &getInstance()
    {
        static T instance;
        return instance;
    }

protected:
    /// @brief default constructor and destructor
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    /// @brief  delete copy constructor and assignment operator
    /// @param  
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

};



}
