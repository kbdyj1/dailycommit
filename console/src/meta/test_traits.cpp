#include <iostream>
#include <type_traits>
#include <functional>
#include <cmath>

namespace { // namespace ======================================================

template <typename T>
struct IsVoid {
    static constexpr bool value = false;
};

template <>
struct IsVoid<void> {
    static constexpr bool value = true;
};

void test_is_void()
{
    std::cout << "IsVoid<int> : " << IsVoid<int>::value << std::endl;
    std::cout << "IsVoid<void> : " << IsVoid<void>::value << std::endl;
}

template <class T>
char test(int T::*);

struct two {
    char c[2];
};
two test(...);

template <class T>
struct IsClass : std::integral_constant<bool, sizeof(test<T>(0)) == 1 && !std::is_union_v<T>>
{};

void test_integral_constant()
{
    typedef std::integral_constant<int, 2> two_t;
    typedef std::integral_constant<int, 4> four_t;

    std::cout << "two_t::value : " << two_t::value << std::endl;
}

template <typename X, typename UnaryOp0, typename UnaryOp1>
X apply_fg(X x, UnaryOp0 op0, UnaryOp1 op1)
{
    return op0(op1(x));
}

template <bool UseSwap>
struct IterSwapImpl;

template <typename ForwardIter0, typename ForwardIter1>
void IterSwap(ForwardIter0 iter0, ForwardIter1 iter1)
{
    typedef std::iterator_traits<ForwardIter0> traits0;
    typedef typename traits0::value_type v0;
    typedef typename traits0::reference r0;
    typedef std::iterator_traits<ForwardIter1> traits1;
    typedef typename traits0::value_type v1;
    typedef typename traits0::reference r1;

    bool const useSwap = std::is_same_v<v0, v1> &&
            std::is_reference_v<v0> &&
            std::is_reference_v<v1>;

    IterSwapImpl<useSwap>::doIt(iter0, iter1);
}

template <>
struct IterSwapImpl<true>
{
    template <typename ForwardIter0, typename ForwardIter1>
    static void doIt(ForwardIter0 iter0, ForwardIter1 iter1)
    {
        std::swap(*iter0, *iter1);
    }
};

template <>
struct IterSwapImpl<false>
{
    template <typename ForwardIter0, typename ForwardIter1>
    static void doIt(ForwardIter0 iter0, ForwardIter1 iter1)
    {
        typename std::iterator_traits<ForwardIter0>::value_type tmp = *iter0;
        *iter0 = *iter1;
        *iter1 = *tmp;
    }
};

class X
{
    int value;

public:
    X() = default;
    X(X&&) noexcept = default;
    X(const X&) = default;
    X& operator=(const X&) = default;
    X& operator=(X&&) noexcept = default;

    void print()
    {
        std::cout << "X::print()" << std::endl;
    }
};

enum E
{
    Valid = 0,
    Invalid
};

union Y
{
    char c;
    int i;
    float f;
    double d;
};

class Z
{
public:
    virtual void print() = 0;
};

template <typename T, bool isReference>
struct addConstRefImpl;

template <typename T>
struct addConstRef
{
    static bool const isReference = std::is_lvalue_reference_v<T>;
    typedef typename addConstRefImpl<T, isReference>::type type;
};

template <typename T>
struct addConstRefImpl<T, true>
{
    typedef T type;
};

template <typename T>
struct addConstRefImpl<T, false>
{
    typedef T const& type;
};

void exam_2_0()
{
    std::cout << "addConstRef<int&> == int& ? " << std::is_same<int&, addConstRef<int&>::type>::value << std::endl;
    std::cout << "addConstRef<int> == const int& ? " << std::is_same<const int&, addConstRef<int>::type>::value << std::endl;
}

} // namespace ================================================================

void test_traits()
{
    int iValue = 6;
    float fValue = 6.3;

    std::cout << "int[5]: " << std::is_array_v<int[5]> << std::endl;
    std::cout << "std::is_class_v<X>: " << std::is_class_v<X> << std::endl;
    std::cout << "std::is_class_v<Y>: " << std::is_class_v<Y> << std::endl;
    std::cout << "std::is_union_v<Y>: " << std::is_union_v<Y> << std::endl;
    std::cout << "std::is_enum_v<E>" << std::is_enum_v<E> << std::endl;
    std::cout << "std::is_integral_v<char>: " << std::is_integral_v<char> << std::endl;
    std::cout << "std::is_member_function_pointer_v<decltype(&X::print)>: " << std::is_member_function_pointer_v<decltype(&X::print)> << std::endl;
    std::cout << "std::is_floating_point_v<decltype(fValue)>: " << std::is_floating_point_v<decltype(fValue)> << std::endl;

    std::cout << "std::is_empty_v<Z>: " << std::is_empty_v<Z> << std::endl;
    std::cout << "std::is_polymorphic_v<Z>: " << std::is_polymorphic_v<Z> << std::endl;

    exam_2_0();
}

void test_blob()
{
    auto a = apply_fg(5.0f, std::negate<float>(), log2);

    std::cout << "apply_fg(5.0f, std::negate<float>(), log2) : " << a << std::endl;

    auto b = apply_fg(3.14f, log2, std::negate<float>());

    std::cout << "apply_fg(3.14f, log2, std::negate<float>()) : " << b << std::endl;
}
