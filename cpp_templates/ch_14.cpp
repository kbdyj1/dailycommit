//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.14 Template instantiation
//
//  22/10/06
//=============================================================================

#include <iostream>
#include "ch_14_t.hpp"

namespace { //=================================================================

template <typename T> class A;

A<int>* p = nullptr;

template <typename T>
class A {
public:
    void f();
};

void g(A<int>& c)
{
    c.f();
}

template <typename T>
void A<T>::f()
{
    std::cout << "A<T>::f()\n";
}

void test()
{
    auto d = A<int>{};

    g(d);

    p->f();
}

//-------------------------------------------------------------------

template <typename T>
class B {
public:
    B(int);
};

void func(B<double>);
void func(int)
{}

void test_func()
{
    func(42);
}

//-------------------------------------------------------------------

namespace _1 {

template <typename T>
void f(T)
{}

template void f<int>(int);
template void f<>(float);
template void f(long);
template void f(char);

template <typename T>
class S {
public:
    void f() {}
};

template void S<int>::f();
template class S<void>;

} //_1 --------------------------------------------------------------

namespace _2 { //c++17

template <typename T>
bool f(T t){
    if constexpr (sizeof(T) <= sizeof(long long)) {
        return 0 < t;
    } else {
        return 0 < t.compare(0);
    }
}

} //_2 --------------------------------------------------------------

namespace _3 { //c++14

template <bool b>
struct Dispatch {
    template <typename T>
    static bool f(T t) {
        return 0 < t.compare(0);
    }
};

template <>
struct Dispatch<true> {
    template <typename T>
    static bool f(T t) {
        return 0 < t;
    }
};

template <typename T>
bool f(T t) {
    return Dispatch<sizeof(T) <= sizeof(long long)>::f(t);
}

} //_3 --------------------------------------------------------------

void test_if_template()
{
    auto n = 10;
    std::cout << "f(" << n << "): ";

#if (0)
    if (_2::f(n)) {
#else
    if (_3::f(n)) {
#endif
        std::cout << "OK\n";
    } else {
        std::cout << "NG\n";
    }
}

template <unsigned long N>
struct Fact {
    static constexpr unsigned long value = N*Fact<N-1>::value;
};
template <>
struct Fact<1> {
    static constexpr unsigned long value = 1;
};

void test_fact()
{
    std::cout << Fact<8>::value << "\n";
}

} //namespace =================================================================

template void externalFunc<int>();  // manual instantiation

void test_ch_14_lazy();
void test_ch_14_instance_model();

void test_ch_14()
{
#if (0) //done
    test();
    test_func();
    test_ch_14_lazy();
    test_ch_14_instance_model();
    test_if_template();
#endif

    test_fact();
}
