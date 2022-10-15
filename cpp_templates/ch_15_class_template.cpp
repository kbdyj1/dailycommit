#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace { //=================================================================

namespace _1 {

template <typename T0, typename T1, typename T2=T1>
class C {
public:
    C(T0 x=T0{}, T1 y=T1{}, T2 z=T2{})
    {}
};

void test()
{
    auto c0 = C{10, 20.0, "Qt"s};
    auto c1 = C{10, 20.0};
    auto c2 = C{"Qt"s, "6.2"};
#if (0)
    auto c3 = C{};  //T0, T1
    auto cr = C{10};//T1
#endif
}

} //_1 --------------------------------------------------------------

namespace _2 {

#define USE_DEDUCE_GUIDE

template <typename T>
struct ValueArg {
    using Type = T;
};

template <typename T>
class S {
    T a;
public:
    using ArgType = typename ValueArg<T>::Type;
#if (0)
    S(T _) : a(_)
#else
    S(ArgType _) : a(_)
#endif
    {}
};

template <typename T>
struct A
{
    T value;
};

#if defined(USE_DEDUCE_GUIDE)
template <typename T> S(T) -> S<T>;
template <typename T> A(T) -> A<T>;
#endif

void test()
{
    S x{10};
    S y(10);
    auto z = S{30};

    S x2{x};
    S x3(x);

#if (0)
    S* p = &x;  // can't form pointer to deduced class template specialization type

    S s0(1), s1(2.0);
#endif

    A<int> a0{10};
    A<int> a2 = {10};

#if (0)
    A<int> a1(10);  //no matching constructor for initialization of 'A<int>'
    A<int> a3 = 10; //no viable conversion from 'int' to A<int>
#endif
}

} //_2 --------------------------------------------------------------

namespace _3 { //implicit deduction guide

template <typename T, typename... Ts>
auto f(T t, Ts... ts)
{
    auto v = std::vector{t, ts...};
    std::cout << typeid(v).name() << ", size: " << v.size() << "\n";
}

void test()
{
    std::vector v0{ 1, 2, 3 };
    std::vector v1{v0, v0};
    std::vector v2{v0};

    std::cout << "v0: " << typeid(v0).name() << "\n";
    std::cout << "v1: " << typeid(v1).name() << "\n";
    std::cout << "v2: " << typeid(v2).name() << "\n";

    f(1, 2, 3, 4);
}

} //_3 --------------------------------------------------------------

namespace _4 { //injected class name

template <typename T>
struct X {
    template<typename Iter>
    X(Iter b, Iter e)
    {}

    template<typename Iter>
    auto f(Iter b, Iter e) {
        return X(b, e);
    }
};

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename T>
struct Y {
    Y(const T&)
    { std::cout << "Y(const T&)\n"; }
    Y(T&&)
    { std::cout << "Y(T&&)\n"; }
};

#if (0)
template <typename T> Y(const T&) -> Y<T>;
template <typename T> Y(T&&) -> Y<T>;
#endif

void g(std::string s)
{
    Y y = s;
}

void test()
{
    g("Qt");
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T, typename U>
struct Z {
    Z(const T&)
    { std::cout << "Z(const T&)\n"; }
    Z(T&&)
    { std::cout << "Z(T&&)\n"; }
};

template <typename T> Z(const T&) -> Z<T, T&>;
template <typename T> explicit Z(T&&) -> Z<T, T>;

void test()
{
    Z z0 = 1;
    Z z1{1};
}

} //_6 --------------------------------------------------------------

namespace _7 {

template <typename... Ts>
struct Tuple {
    Tuple(Ts...)
    { std::cout << "Tuple(Ts...)\n"; }

    Tuple(const Tuple<Ts...>&)
    { std::cout << "Tuple(const Tuple<Ts...>&)\n"; }
};

template <typename... Ts> Tuple(Ts...) -> Tuple<Ts...>;
template <typename... Ts> Tuple(const Tuple<Ts...>&) -> Tuple<Ts...>;

void test()
{
    auto t0 = Tuple{1, 2};
    Tuple t1(t0);
    Tuple t2{t0, t0};
    Tuple t3{t0};

    auto e = Tuple(t0);
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_class_template()
{
#if (0) //done
    _3::test();
    _5::test();
    _6::test();
#endif

    _7::test();
}
