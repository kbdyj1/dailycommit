//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.15 Template argument deduction
//
//  22/10/08
//=============================================================================

#include <iostream>
#include <vector>
#include <initializer_list>

namespace { //=================================================================

template <typename C>
typename C::value_type at(C container, int i)
{
    return container[i];
}

void test_at()
{
#if (0)
    int array[8] = {0,};
#else
    auto array = std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7};
#endif

    auto v = at(array, 0);

    std::cout << "at(0): " << v << "\n";
}

namespace _1 {

template <typename T>
void f(T)
{}

template <typename T>
void g(T&)
{}

void test()
{
    double array[32];
    const int N = 1;

    f(array);   //double*
    g(array);   //double[32]
    f(N);       //int
    g(N);       //const int
    f(1);       //int
#if (0)
    g(1);       //error
#endif
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
void f0(T*)
{}

template <typename E, int N>
void f1(E(&)[N])
{}

template <typename T0, typename T1, typename T2>
void f2(T0(T1::*)(T2*))
{}

class A {
public:
    void f(double*) {}
};

void g(int*** p)
{
    bool b[2];

    f0(p);  //T=int**
    f1(b);  //T=bool, N=2
    f2(&A::f);  // T0=void, T1=A, T2=double
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <int N>
class X {
public:
    using I = int;

    void f(int) {}
};

template <int N>
void f(void (X<N>::*p)(typename X<N>::I))
{}

void test()
{
    f(&X<1>::f);
}

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename T>
void f(T, T)
{}

void (*pf)(char, char) = &f;

} //_4 --------------------------------------------------------------

namespace _5 {

class S {
public:
    template <typename T>
    operator T&()
    {
        return *this;
    }
};

void f(int(&)[20])
{}

void g(S s)
{
    f(s);
}

void test()
{
    auto s = S{};

    g(s);
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
void f(T t)
{
    std::cout << "f(T)\n";
}

template <typename T>
void f(std::initializer_list<T>)
{
    std::cout << "f(std::initializer_list<T>)\n";
}

void test()
{
    f({1, 2, 3});
}

} //_6 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15()
{
#if (0) //done
    test_at();
    _3::test();
    _5::test();
#endif

    _6::test();
}
