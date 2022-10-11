#include <iostream>

namespace { //=================================================================

namespace _1 {

template <typename T>
class B
{};

template <typename T>
class D : public B<T>
{};

template <typename T>
void f(B<T>*)
{}

void g(D<long> d)
{
    f(&d);
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
int f(T, typename T::X)
{}

struct V {
    V(){}
    struct X {
        X(double){}
    };
};

void test()
{
    auto v = V{};
    f(v, 1.0);
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T>
class A {
    T a;
public:
    A(T t = T{}) : a(t)
    {}
};

void test()
{
#if 201402L < __cplusplus //c++17
    auto a = A(10);
#else
    auto a = A<int>(10);
#endif
}

} //_3 --------------------------------------------------------------

namespace _4 {

class A {
public:
    A(int, int)
    {}
};

template <typename T>
void init(T* p, const T& value = T{})
{
    *p = value;
}

template <typename T>
void f(T t = 0)
{}

void test()
{
    auto a = A{10, 10};

    init(&a, A{20, 20});

    f<int>();
#if (0)
    f();
#endif
}

} //_4 --------------------------------------------------------------

namespace _5 { // exception spec

#if (0)
template <typename T>
void f(T, int) noexcept(nonexistent(T()))
{}

template <typename T>
void f(T, ...)
{}


template <typename T>
void g(T, int) throw(typename T::Nonexistent)
{}

template <typename T>
void g(T, ...)
{}

void test(int i)
{
    f(i, i);
    g(i, i);
}
#endif

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
T defaultValue()
{
    return T{};
}

template <typename Out, typename In>
Out convert(In in)
{
    std::cout << "in: " << typeid(in).name() << ", out: " << typeid(Out).name() << "\n";
    return Out(in);
}

int f(int)
{
    std::cout << "int f(int)\n";
}
template <typename T>
T f(T)
{
    std::cout << "T f<T>(T)\n";
}

void test()
{
    auto value = defaultValue<int>();

    auto ret = convert<double>(40);

    auto x = f(10);
    auto y = f<>(10);
}

} //_6 --------------------------------------------------------------

namespace _7 {

void f()
{}

template <typename>
void f()
{}

namespace N {

class C {
public:
    friend int f();
#if (0)
    friend int f<>();
#endif
};

} //N

template <typename T>
typename T::ReturnType g()
{}

template <typename T>
T g()
{
    return T{};
}

template <typename T>
void h(T)
{}

template <typename T>
void h(T, T)
{}

template <typename... Ts>
void j(Ts... ts)
{}

void test()
{
    auto x = g<int*>;
#if (0)
    auto y = h<int*>;
#endif

    j(1, 2.0, 3);
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_limit()
{
#if (0)
    _6::test();
#endif
    _7::test();
}
