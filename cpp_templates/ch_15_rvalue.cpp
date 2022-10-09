#include <iostream>
#include <utility>

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

} //namespace =================================================================

void test_ch_15_rvalue()
{
#if (0) //done
    test_reference_collapsing_rule();
    test_forward_reference();
#endif

    test_forward();
}
