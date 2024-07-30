#pragma once


/// @brief reference by https://zhuanlan.zhihu.com/p/362173165
namespace TestDemo
{

template <typename R, typename T, typename... Args>
struct AnyMemberFunCall
{
    using FunType = R(T::*)(Args...);
    AnyMemberFunCall(T* pkObj, FunType pfFun):m_pkObj(pkObj),m_pfFun(pfFun) {};
    R DoInvoke(Args&&... args){return (m_pkObj->*m_pfFun)(std::forward<Args>(args)...);};
private:
    T* m_pkObj = nullptr;
    FunType m_pfFun = nullptr;
};


template <typename R, typename T, typename... Args>
auto CreateAnyMemberFunCall(T* pkObj, R(T::*pfFunc)(Args...)) -> decltype(AnyMemberFunCall(pkObj, pfFunc))
{
    return AnyMemberFunCall(pkObj, pfFunc);
}

template <typename T>
struct MyEnableIf
{
    using type = void;
};

template <typename T>
struct MyEnableIf<true, T>
{
    using type = T;
};




}
