#include <iostream>
#include <type_traits>

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

} // namespace ================================================================

void test_traits()
{
    test_is_void();
    test_integral_constant();
}
