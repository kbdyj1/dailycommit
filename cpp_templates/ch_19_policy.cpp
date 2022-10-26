#include <iostream>
#include <type_traits>
namespace { //=================================================================

namespace _1 {

template <typename T>
struct RParam {
    using Type = typename std::conditional<sizeof(T) <= 2*sizeof(void*), T, const T&>::type;
};

class A {
public:
    A() {
        std::cout << "A()\n";
    }
    A(const A&) {
        std::cout << "A(const A&)\n";
    }
};

class B {
public:
    B() {
        std::cout << "B()\n";
    }
    B(const B&) {
        std::cout << "B(const B&)\n";
    }
};

template <>
struct RParam<B> {
    using Type = B;
};

template <typename T0, typename T1>
void fooImpl(typename RParam<T0>::Type t0, typename RParam<T1>::Type t1)
{}

template <typename T0, typename T1>
void foo(T0&& t0, T1&& t1)
{
    fooImpl<T0, T1>(std::forward<T0>(t0), std::forward<T1>(t1));
}

void test()
{
    auto a = A{};
    auto b = B{};

    foo(a, b);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_19_policy()
{
    _1::test();
}
