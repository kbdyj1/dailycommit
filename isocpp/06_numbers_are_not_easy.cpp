#include <iostream>
#include <typeinfo>
#include <vector>
#include <utility>
#include <cstdint>
#include <numeric>
#include <cmath>
#include <concepts>

namespace { //=================================================================

namespace _1 {

void test_promotion()
{
    uint16_t a = 1;
    uint16_t b = 2;

    auto v = a - b;

    std::cout << typeid(v).name() << "\n";
}

void test_conversions()
{
    int a = -100;
    long int b = 500;

    auto v = a + b;

    std::cout << typeid(v).name() << "\n";
}

void test_mixed_signedness()
{
    {
        int a = -100;
        unsigned b = 0;
        auto v = a + b;

        std::cout << typeid(v).name() << "\n";
    }
    {
        unsigned a = 100;
        long int b = -200;
        auto v = a + b;

        std::cout << typeid(v).name() << "\n";
    }
    {
        long long a = -100;
        unsigned long b = 0;
        auto v = a + b;

        std::cout << typeid(v).name() << "\n";
    }
    {
        int x = -1;
        unsigned y = 1;

        std::cout << "-1 > 1 ? " << (x > y) << "\n";
    }
}

void test()
{
#if (0) //done
    test_promotion();
    test_conversions();
#endif

    test_mixed_signedness();
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test_ssize()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    for (ptrdiff_t i = 0; i < std::ssize(v); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
}

void test_safe_integral_comparisions()
{
    int x = -1;
    unsigned y = 1;
    long z = -1;

    auto r0 = x > y;
    auto r1 = std::cmp_greater(x, y);
    std::cout << "x > y ? " << r0 << "\n";
    std::cout << "cmp_greater(x, y) ? " << r1 << "\n";

    auto r2 = z < y;
    auto r3 = std::cmp_less(z, y);

    std::cout << "z < y ? " << r2 << "\n";
    std::cout << "cmp_less(z, y) ? " << r3 << "\n";
}

void test_in_range()
{
    std::cout << "std::in_range<unsigned>(-1) ? " << std::in_range<unsigned>(-1) << "\n";
}

void test()
{
#if (0) //done
    test_ssize();
    test_safe_integral_comparisions();
#endif

    test_in_range();
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test_floating_point()
{
    auto src = UINT64_MAX - UINT32_MAX;
    auto m = (1.0f * src) * 1.0L;
    auto n = 1.0f * (src * 1.0L);

    std::cout << std::fixed << "m: " << m << "\nn: " << n << "\nsrc: " << src << "\n";
}

void test_ordering() //???
{
    float v = 1.0f;
    float next = std::nextafter(v, 2.0);
    float diff = (next -v) / 2;

    std::cout << "diff: " << diff << ", next: " << next << "\n";

    auto data0 = std::vector<float>(100, diff);
    data0.front() = v;
    float r0 = std::accumulate(data0.begin(), data0.end(), 0.f);

    std::cout << "r0: " << r0 << "\n";

    auto data1 = std::vector<float>(100, diff);
    data1.back() = v;
    float r1 = std::accumulate(data1.begin(), data1.end(), 0.f);

    std::cout << "r1: " << r1 << "\n";
}

void function(const int& v)
{}

void test_reference()
{
    long a = 0;
    long long b = 0;

    static_assert (sizeof(a) == sizeof(b));

    function(a);
    function(b);
}

void test()
{
#if (0) //done
    test_floating_point();
#endif

    test_ordering();
}

} //_3 --------------------------------------------------------------

namespace _4 {

void test_type_deduction()
{
    auto v = std::vector<unsigned>{ 1, 2, 3, 4, 5 };
    auto sum = std::accumulate(v.begin(), v.end(), 0);

    std::cout << typeid(sum).name() << "\n";
}

void test()
{
    test_type_deduction();
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename T>
concept IsInt = std::same_as<int, T>;

void function(const IsInt auto&)
{}

void test()
{
    function(0);
#if (0) //compile error
    function(0u);
#endif
}

} //_5 --------------------------------------------------------------

} //===========================================================================

void test_numbers_are_not_easy()
{
    std::cout << std::boolalpha;

#if (0) //done
    _1::test();
    _2::test();
    _3::test();
#endif

    _4::test();
}
