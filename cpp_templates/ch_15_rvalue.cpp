#include <iostream>
#include <utility>
#include <type_traits>

namespace { //=================================================================

void test_reference_collapsing_rule()
{
    using RI = int&;
    {
        int i = 62;
        RI r = i;
        const RI& rr = r;
    }
    {
        using CRI = const int&;
        volatile CRI&& r = 42;

        using RRI = int&&;
        const RRI&& rr = 42;
    }
}

template <typename T>
void f(T&& t)
{
#if (0)
    T x;    //error: ‘x’ declared as reference but not initialized
#endif
}

void test_forward_reference()
{
    int i = 0;
    const int j = 0;
    f(i);   //int&
    f(j);   //const int&
    f(4);   //int&&
}

class C
{};

void g(C&)
{
    std::cout << "g(C&)\n";
}
void g(const C&)
{
    std::cout << "g(const C&)\n";
}
void g(C&&)
{
    std::cout << "g(C&&)\n";
}

template <typename T>
void forwardToG(T&& t)
{
#if (0)
    g(static_cast<T&&>(t));
#else
    g(std::forward<T>(t));
#endif
}

void test_forward()
{
    auto v = C{};
    const auto c = C{};

    forwardToG(v);
    forwardToG(c);
    forwardToG(C{});
    forwardToG(std::move(v));
}

namespace variadic {

template <typename T>
void h(T&& t)
{
    std::cout << typeid(t).name() << ", is_rvalue_reference: " << std::is_rvalue_reference<T>::value << "\n";
}

template <typename T>
void forwardToH(T&& t)
{
    h(std::forward<T>(t));
}

template <typename T, typename... Ts>
void forwardToH(T&& t, Ts&&... ts)
{
    h(std::forward<T>(t));
    if (sizeof...(ts)) {
        forwardToH(ts...);
    }
}

void test_variadic_forward()
{
    auto i = 10;
    auto s = std::string{"Hello, Qt"};
    auto d = 20.0;
    forwardToH(i, s, d, C{});
}

void g(int*)
{
    std::cout << "g(int*)\n";
}
void g(...)
{
    std::cout << "g(...)\n";
}

template <typename T>
void forwardToG(T&& t)
{
    g(std::forward<T>(t));
}

void test()
{
    g(0);
    forwardToG(0);

    g(nullptr);
    forwardToG(nullptr);
}

template <typename... Ts>
#if 201103L < __cplusplus //c++14~
    decltype(auto) forwardToA(Ts&&... ts)
#else
    auto forwardToA(Ts&&... ts) -> decltype(a(std::forward<Ts>(ts)...))
#endif
    {
        return a(std::forward<Ts>(ts)...);
    }

} //variadic --------------------------------------------------------

namespace surprise {

void intLvalues(int&)
{}

template <typename T>
void lvalues(T&)
{}

void intRvalues(int&&)
{}

template <typename T>
void anything(T&&)
{}

template <typename T>
class X
{
public:
    X(X&&);
    X(T&&);
    template<typename U>
    X(X<U>&&);
    template<typename U>
    X(U, T&&);

    template<typename U>
    typename std::enable_if_t<!std::is_lvalue_reference<U>::value>
    rvalues(U&&);
};

void test()
{
    auto n = 10;
    intLvalues(n);
    lvalues(n);
    anything(n);

    intRvalues(10);
    anything(C{});
}

} //surprise --------------------------------------------------------

} //namespace =================================================================

void test_ch_15_rvalue()
{
#if (0) //done
    test_reference_collapsing_rule();
    test_forward_reference();
    test_forward();
    variadic::test_variadic_forward();
#endif
    variadic::test();
}
