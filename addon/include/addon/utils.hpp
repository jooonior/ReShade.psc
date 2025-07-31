#pragma once

template <typename T>
struct signature
{
	using type = typename signature<decltype(&T::operator())>::type;
};

template <typename R, typename... Args>
struct signature<R(Args...)>
{
	using type = R(Args...);
};

template <typename R, typename... Args>
struct signature<R(*)(Args...)>
{
	using type = R(Args...);
};

template <typename R, typename C, typename... Args>
struct signature<R(C:: *)(Args...)>
{
	using type = R(Args...);
};

template <typename R, typename C, typename... Args>
struct signature<R(C:: *)(Args...) const>
{
	using type = R(Args...);
};

template <typename T>
using signature_t = typename signature<T>::type;
