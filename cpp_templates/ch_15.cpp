//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.15 Template argument deduction
//
//  22/10/08
//=============================================================================

#include <iostream>
#include <vector>
#include <initializer_list>
#include <array>

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

namespace _7 { // parameter pack

void f()
{
    std::cout << "\n";
}

template <typename T, typename... Pack>
void f(T t, Pack... pack)
{
    std::cout << typeid(t).name();
    if (0 < sizeof...(pack)) {
        std::cout << ", ";
    }
    f(pack...);
}

void test()
{
    auto n = 0;
    auto d = 1.0;
    auto*p = &n;

    f(n, d, p);
}

template <typename T, typename U>
class Pair
{};

template <typename T, typename... Ts>
void g0(const Pair<T, Ts>&...)
{
    std::cout << "g0() ";
}

template <typename... Ts, typename... Us>
void g1(const Pair<Ts, Us>&...)
{
    std::cout << "g1() ";
}

void test2()
{
    auto p0 = Pair<int, float>{};
    auto p1 = Pair<int, double>{};
    auto p2 = Pair<double, double>{};

    g0(p0, p1);
    g1(p0, p1);
//    g0(p0, p2);
    g1(p0, p2);
}

} //_7 --------------------------------------------------------------

namespace _8 {

template <typename... Ts>
class Tuple
{};

template <typename... Ts>
bool f0(Tuple<Ts...>, Tuple<Ts...>)
{
    return true;
}

template <typename... Ts, typename... Us>
bool f1(Tuple<Ts...>, Tuple<Us...>)
{
    return true;
}

void test()
{
    auto t0 = Tuple<short, int, long>{};
    auto t1 = Tuple<unsigned short, unsigned, unsigned long>{};

    f0(t0, t0);
    f1(t0, t0);
//    f0(t0, t1);
    f1(t0, t1);
}

} //_8 --------------------------------------------------------------

namespace _9 { // literal operator

template <char... cs> int operator"" _B7()
{
    auto array = std::array<char, sizeof...(cs)>{cs...};

    for (auto c : array) {
        std::cout << "'" << c << "'";
    }
    std::cout << "\n";

    return 0;
}

void test_b7()
{
    auto a = 120_B7;
}

} //_9 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_rvalue();
void test_ch_15_sfinae();
void test_ch_15_limit();
void test_ch_15_auto();

void test_ch_15()
{
#if (0) //done
    test_at();
    _3::test();
    _5::test();
    _6::test();
    _7::test();
    test_ch_15_rvalue();
    test_ch_15_sfinae();
    test_ch_15_limit();
#endif

    test_ch_15_auto();
}
