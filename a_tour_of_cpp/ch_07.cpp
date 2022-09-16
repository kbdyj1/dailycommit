//=============================================================================
//  A tour of C++
//  Ch.06  Concept & Generic Programming
//
//  22/09/15
//=============================================================================

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <iterator>

namespace { //=================================================================

using namespace std::string_literals;

template <typename C, typename V>
#if 201703L < __cplusplus
requires std::is_arithmetic_v<V>
#endif
V sum(const C& c, V value) {
    for (const auto& i : c) {
        value += i;
    }
    return value;
}

#if 201703L < __cplusplus
template <typename T, typename T2 = T>
concept equality_compare = requires (T a, T2 b) {
    { a == b } -> bool;
    { a != b } -> bool;
    { b == a } -> bool;
    { b != a } -> bool;
};

void test_equality_compare()
{
    static_assert(equality_compare<int, double>);
    static_assert(equality_compare<int>);
    static_assert(equality_compare<int, std::string>);
}

#endif

void print()
{
    std::cout << std::endl;
}

template <typename T, typename... Tail>
void print(T head, Tail... tail) {
    std::cout << head << ' ';
    if constexpr(sizeof...(tail) > 0)
        print(tail...);
}

void test_print()
{
    print("Hello", "Qt", 6, 2.0);
}

#if 201703L == __cplusplus
template <typename... T>
int sum2(T... v)
{
#if (0)
    return (v + ... + 0);   // right
#else
    return (0 + ... + v);   // left
#endif
}

void test_sum2()
{
    std::cout << sum2(1, 2, 3, 4, 5.5);
}

template <typename... T>
void print2(T&&... args)
{
    (std::cout << ... << args) << std::endl;
}

void test_print2()
{
    print2("Hello"s, "Qt"s, 6, 2.0);
}

#endif

template <typename Res, typename... Ts>
std::vector<Res> to_vector(Ts&&... ts)
{
    auto res = std::vector<Res>{};

    (res.push_back(ts), ...);

    return res;
}

void test_to_vector()
{
    auto v = to_vector<double>(1, 2, 4.5, 'a');
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<double>(std::cout, " "));
    std::cout << std::endl;
}

} //namespace =================================================================

void test_ch_07()
{
#if (0) // done
    test_print();
    test_sum2();
    test_print2();
#endif

#if 201703L == __cplusplus
#endif

    test_to_vector();
}
