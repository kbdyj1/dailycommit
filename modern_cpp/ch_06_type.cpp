#include <iostream>
#include <type_traits>
#include <string>

#include "util.h"

using namespace std::string_literals;

namespace { //=================================================================

template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
T mul(const T t0, const T t1)
{
    return t0 * t1;
}

void test_mul()
{
    auto i0 = 2;
    auto i1 = 3;
    auto i2 = mul(i0, i1);

    auto s0 = "Hello,"s;
    auto s1 = "Qt6"s;
    //auto s2 = mul(s0, s1);
}

template <typename T>
struct PodWrapper {
    static_assert(std::is_pod<T>::value, "not POD");
    T value;
};

struct A {
    int iValue;
    double dValue;
    char array[8];
};
struct B {
    int id;
    std::string name;
};

void test_pod()
{
    auto p0 = PodWrapper<A>();
    //auto p1 = PodWrapper<B>();
}

template <typename T>
struct ConstWrapper {
    typedef typename std::conditional<
        std::is_const<T>::value,
        T,
        typename std::add_const<T>::type>::type const_type;
};

void test_conditional()
{
    static_assert(std::is_const<ConstWrapper<int>::const_type>::value);
    static_assert(std::is_const<ConstWrapper<const int>::const_type>::value);
}

template <typename T>
auto process(T arg)
{
    if constexpr (std::is_same<T, bool>::value) {
        return !arg;
    } else if constexpr (std::is_integral<T>::value) {
        return -arg;
    } else if constexpr (std::is_floating_point<T>::value) {
        return std::abs(arg);
    } else {
        return arg;
    }
}

void test_constexpr()
{
    PRINT_FUNC(process(true));
    PRINT_FUNC(process(2));
    PRINT_FUNC(process(6.2));
    PRINT_FUNC(process("Hello, Qt6"s));
}

template <typename T>
struct IsVoid {
    static const bool value = false;
};

template <>
struct IsVoid<void> {
    static const bool value = true;
};

template <typename T>
struct IsPointer {
    static const bool value = false;
};

template <typename T>
struct IsPointer<T*> {
    static const bool value = true;
};

struct C {
    int value;
};

void test()
{
    std::cout << std::boolalpha;

    PRINT_FUNC(IsVoid<C>::value);
    PRINT_FUNC(IsVoid<void>::value);
    PRINT_FUNC(IsVoid<void()>::value);

    PRINT_FUNC(IsPointer<C*>::value);
}

} //namespace =================================================================

void test_ch_06_type()
{
#if (0) // done
    test_constexpr();
#endif

    test();
}
