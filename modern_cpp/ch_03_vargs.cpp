#include <iostream>
#include <string>
#include <tuple>

namespace { //=================================================================

using namespace std::literals;

#if (0)
#   define RET T
#else
#   define RET auto
#endif

#define CXX17_FOLDING

#ifndef CXX17_FOLDING
template <typename T>
RET add(T value)
{
    std::cout << "T add(" << value << ")\n";
    return value;
}

template <typename T, typename... Ts>
RET add(T head, Ts... rest)
{
    std::cout << "T add(" << head << ", rest...)\n";
    return head + add(rest...);
}
#else
template <typename... Ts>
auto add(Ts... args)
{
#if (0)
    return (... + args);    // left folding
#else
    return (args + ...);    // right folding
#endif
}
#endif

void test_varg_add()
{
    auto res = add(1, 2, 3, 4, 5);
    std::cout << "add(1, 2, 3, 4, 5): " << res << "\n";

    auto str = add("Hello"s, ',', " Qt "s, '6', ".0!!!"s);
    std::cout << str << "\n";
}

template <typename... T>
auto makeEvenTuple(T... a)
{
    static_assert (sizeof...(a) % 2 == 0, "expected an even number of arguments");
    return std::tuple<T...>{a...};
}

void test_even_tuple()
{
    auto t0 = makeEvenTuple(1, 2, 3, 4);
    //auto t1 = makeEvenTuple(1, 2, 3);
}

template <typename T>
struct Wrapper {
    const T& value;
};

template <typename T>
constexpr auto operator<(const Wrapper<T>& l, const Wrapper<T>& r)
{
    return Wrapper<T>{
        l.value < r.value ? l.value : r.value
    };
}
template <typename... Ts>
constexpr auto min(Ts&&... args)
{
    return (Wrapper<Ts>{args} < ...).value;
}

void test_min()
{
    auto m = min(1, 3, 5, 7, 9, 2, 4, 6, 8, 10);

    std::cout << "min: " << m << "\n";
}

} //namespace =================================================================

void test_ch_03_vargs()
{
    //test_varg_add();
    test_min();
}
