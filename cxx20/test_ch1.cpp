#include <concepts>
#include <iostream>
//#include <ranges>
#include <vector>
#include <span>
#include <cmath>
#include <chrono>
#include <string>
#include <utility>

#if (0) // not supported in my environment.
export module ch1;

export int add(int a, int b)
{
    return a + b;
}
#endif

namespace
{

/******************************************************************************
 * concepts
 */
template <typename T>
concept Integral = std::is_integral<T>::value;

Integral auto gcd(Integral auto a, Integral auto b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}
void test_concepts()
{
    int a = 10;
    int b = 15;

    std::cout << "gcd(" << a << ", " << b << "):" << gcd(a, b);

#if (0)
    //note: the expression ‘std::is_integral<_Tp>::value [with _Tp = float]’ evaluated to ‘false’
    //    5 | concept Integral = std::is_integral<T>::value;

    float c = 10.0;
    float d = 15.0;

    std::cout << "gcd(" << c << ", " << d << "):" << gcd(c, d);
#endif
}

/******************************************************************************
 * constexpr
 */
consteval int sqr(int n) {
    return n * n;
}

void test_consteval()
{
    constexpr auto r = sqr(100);

#if (0) // x is not const
    int x = 100;
    auto r2 = sqr(x);
#endif
}

class Point
{
public:
    int x;
    int y;
    int z;
};

/******************************************************************************
 * initialize
 */
void print_point(Point p)
{
    std::cout << "Point(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
}

void test_initialize()
{
    auto p0 = Point { .x = 1, .z = 3 };
    print_point(p0);

    //warning: ISO C++ requires field designators to be specified in declaration order; field 'z' will be initialized after field 'y' [-Wreorder-init-list]
    //    auto p1 = Point { .x = 1, .z = 3, .y = 2 };
    //auto p1 = Point { .x = 1, .z = 3, .y = 2 };
    //print_point(p1);
}

void test_ranges()
{
    auto ints = std::vector<int>{ 0, 1, 2, 3, 4, 5 };
    auto even = [](int i) { return i % 2 == 0; };
    auto squire = [](int i) { return i * i; };
#if (0) // not supported yet
    for (auto i : ints | std::views::filter(even) | std::views::transform(squire)) {
        std::cout << i << ' ';
    }
#endif
}

/******************************************************************************
 * template
 */
struct Implicit {
    template<typename T>
    Implicit(T t) {
        std::cout << t << std::endl;
    }
};

struct Explicit {
    template<typename T>
    explicit Explicit(T t) {
        std::cout << t << std::endl;
    }
};

void test_template()
{
    Implicit imp = "implicit constructor";
    Explicit exp = Explicit{"explicit constructor"};
}

/******************************************************************************
 * span
 */
void copy(std::span<const int> src, std::span<int> dest)
{
    std::cout << "src:" << src.data() << ", len:" << src.size() << std::endl;
    std::cout << "dest:" << dest.data() << ", len:" << dest.size() << std::endl;
}
void test_span()
{
    int src[4] = { 0, 1, 2, 3 };
    int dest[4];

    copy(src, dest);
}

/******************************************************************************
 * math util
 */
void test_math_util()
{
    int x = -5;
    unsigned int y = 10;

    std::cout << "x(" << x << "), y(" << y << ") : " << std::endl;
    std::cout << "x(unsigned int): " << static_cast<unsigned int>(x) << std::endl;
    std::cout << "y(int): " << static_cast<int>(y) << std::endl;

    if (x < y) {
        std::cout << " x<y: true";
    } else {
        std::cout << " x<y: false";
    }
    std::cout << std::endl;

#if !defined(OS_MAC)
    if (std::cmp_less(x, y)) {
        std::cout << "std::cmp_less(x,y): true";
    } else {
        std::cout << "std::cmp_less(x,y): false";
    }
    std::cout << std::endl;
#endif
}

/******************************************************************************
 * formatting
 */

void test_formatting()
{
    //auto message = std::format("The answer is {}.", 80); // not yet?
}

}
//=============================================================================

void test_ch1()
{
    //test_concepts();
    //test_consteval();
    //test_initialize();
    //test_template();
    //test_span();
    test_math_util();
}
