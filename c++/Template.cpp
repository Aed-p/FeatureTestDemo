// #include <common/stack_trace.h>
#include <iostream>
#include <functional>

namespace TestDemo
{
/// Releated to std::any, unique typeinfo could store any type info by static constexpr id address compare
template <typename T>
struct unique_typeinfo
{
    static constexpr int id = 0;
};

template <typename T>
constexpr int unique_typeinfo<T>::id;

/// Simple implementation, does not consider reference related types
template <typename T, typename U>
inline constexpr bool compare()
{
    return &unique_typeinfo<T>::id == &unique_typeinfo<U>::id;
}

/// question: https://www.zhihu.com/question/472749525/answer/2084936755

template <typename F, typename... Args>
void process(F&& func, Args &&... args)
{
    func(std::forward<Args>(args)...);
}
}

void print(const std::string & test)
{
    std::cout << test << std::endl;
}

// void print(const std::string & test, const std::string & test_tep)
// {
//     std::cout << test << test_tep << std::endl;
// }

// template <typename R, typename T, typename... Args>
// struct AnyMatchFunCall
// {
//     using FuncType = R(T::*)(Args &&...);
//     AnyMatchFunCall(T* pkObj_, FuncType pfFunc_) : pkObj(pkObj_), pfFunc(pfFunc_) {}
//     D Invoke(Args &&... args) { return (pkObj->*pfFunc(std::forward<Args>(args)...)); }

// private:
//     T* pkObj = nullptr;
//     FuncType pfFunc = nullptr;
// };

// template <typename R, typename T, typename... Args>
// auto createAnyMatchFuncCall(T* pkObj, R(T::*pkFun)(Args...)) -> decltype(AnyMatchFunCall(pkObj, pkFun))
// {
//     return AnyMatchFunCall(pkObj, pkFun);
// }

int main()
{
    /// print
    // TestDemo::process(print, "Hello world");
    std::function<void(const std::string &)> func = print;
    TestDemo::process(func, "Hello world");
}