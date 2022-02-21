#include <concepts>
#include <list>
#include <algorithm>
#include <iostream>

namespace
{

void test_sort()
{
    auto l = std::list<int>{ 0, 10, 2, 13, 40, 25 };
#if (0)
    std::sort(l.begin(), l.end());
#endif
}

template <typename T>
concept integral = std::is_integral_v<T>;

template <typename T>
requires integral<T>
auto gcd(T a, T b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

template <typename T>
auto gcd1(T a, T b) requires integral<T>
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

template <integral T>
auto gcd2(T a, T b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// abbreviated function templates syntax
integral auto gcd3(integral auto a, integral auto b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// abbreviated function template synatax
auto gcd4(auto a, auto b)
{
    if (b == 0)
        return a;
    else
        return gcd4(b, a % b);
}

template <unsigned int i>
requires (i <= 20)
int sum(int j) {
    return i + j;
}

void test_sum()
{
    std::cout << "sum<20>(200) : " << sum<20>(200) << std::endl;

#if (0)
    //    ../cxx20/test_ch3.cpp:57:5: note: candidate template ignored: constraints not satisfied [with i = 21]
    //    int sum(int j) {
    //        ^
    //    ../cxx20/test_ch3.cpp:56:11: note: because '21U <= 20' (21 <= 20) evaluated to false
    //    requires (i <= 20)
    //              ^
    std::cout << "sum<21>(200) : " << sum<21>(200) << std::endl;
#endif
}

/******************************************************************************
 * overload
 */
void overload(auto t)
{
    std::cout << "auto : " << t << std::endl;
}

void overload(integral auto t)
{
    std::cout << "integral : " << t << std::endl;
}

void overload(long t)
{
    std::cout << "long : " << t << std::endl;
}

void test_overload()
{
    std::cout << std::endl;

    overload(3.14);
    overload(2010);
    overload(2020L);

    std::cout << std::endl;
}

/******************************************************************************
 * same_as
 */
template <typename T, typename ... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

template <typename T>
concept IsPrintable = std::integral<T> || std::floating_point<T> ||
        IsAnyOf<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>, char, wchar_t>;

void println(IsPrintable auto const ... args)
{
    (std::wcout << ... << args) << std::endl;
}

void test_same_as()
{
    println("Example: ", 3.14, " : ", 42, " : [", 'a', L'-', L"Z]");
}

/******************************************************************************
 * derived_from
 */
class A{};
class B : public A{};
class C : private A{};

void test_derived_from()
{
    static_assert(std::derived_from<B, B> == true);
    static_assert(std::derived_from<int, int> == false);
    static_assert(std::derived_from<B, A> == true);
    static_assert(std::derived_from<C, A> == false);

    static_assert(std::is_base_of<B, B>() == true);
    static_assert(std::is_base_of<int, int>() == false);
    static_assert(std::is_base_of<A, B>() == true);
    static_assert(std::is_base_of<A, C>() == true);
}

} // namespace

void test_ch3()
{
    //test_sort();
    //test_sum();
    //test_overload();
    //test_same_as();
    test_derived_from();
}
