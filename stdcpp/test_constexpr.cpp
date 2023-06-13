#include <iostream>
#include <algorithm>
#include <iterator>
#include <array>

namespace { //=================================================================

namespace _1 {

void test()
{
    constexpr int ArraySize = 4;
    std::array<int, ArraySize> data = { 1, 2, 3, 4 };
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));

    std::cout << std::endl;
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    int sz;
    const auto ArraySize = sz;

#if (0)
    //error: the value of ‘ArraySize’ is not usable in a constant expression
    //   29 |     std::array<int, ArraySize> data;
    //      |                     ^~~~~~~~~

    std::array<int, ArraySize> data;
#endif
}

} //_2 --------------------------------------------------------------

namespace _3 {

constexpr int pow(int base, int exp) noexcept
{
#if __cplusplus < 201402L
    return (exp == 0) ? 1 : base * pow(base, exp-1);
#else
    auto result = 1;
    for (int i=0; i<exp; i++)
        result *= base;
    return result;
#endif
}

void test()
{
    constexpr auto Conds = 5;
    std::array<int, pow(3, Conds)> data;
    std::cout << "std::array<int, pow(3, Conds)>.size(): " << data.size() << std::endl;
}

} //_3 --------------------------------------------------------------

namespace _4 {

class Point {
    double mx;
    double my;
public:
    constexpr Point(double x, double y) noexcept : mx(x), my(y)
    {}
    constexpr double x() const noexcept { return mx; }
    constexpr double y() const noexcept { return my; }

#if __cplusplus < 201402L
    void setX(double x) { mx = x; }
    void setY(double y) { my = y; }
#else
    constexpr void setX(double x) noexcept { mx = x; }
    constexpr void setY(double y) noexcept { my = y; }
#endif
};

constexpr Point mid(const Point& p0, const Point& p1) noexcept
{
    return { (p0.x() + p1.x()) / 2, (p0.y() + p1.y()) / 2 };
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    os << "Point(" << p.x() << ", " << p.y() << ")";
    return os;
}

void test()
{
    constexpr auto p0 = Point{1.0, 2.0};
    constexpr auto p1 = Point{3.0, 4.0};

    constexpr auto p2 = mid(p0, p1);

    std::cout << p2 << std::endl;

    constexpr std::array<int, (int)p2.x()> data { 1, 2 };

    std::cout << "data.size(): " << data.size() << std::endl;
}

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_constexpr()
{
#if (0) //done
    _1::test();
    _3::test();
#endif

    _4::test();
}
