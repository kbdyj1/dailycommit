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

}

void test_ch3()
{
    //test_sort();
    //test_sum();
    test_overload();
}
