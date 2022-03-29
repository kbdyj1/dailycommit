#include <iostream>
#include <vector>
#include <algorithm>
#include <span>
#include <iterator>

namespace { //=================================================================

// P1. Express ideas directly in code

class Date
{
    enum class Month
    {
        Jan = 0,
    };

public:
    Month month() const
    {
        return mMonth;
    }

private:
    Month mMonth;
};

void f(std::vector<std::string>& v)
{
    std::string val;
    std::cin >> val;

#if (0)
    int index = -1;
    for (int i=0; i<v.size(); i++) {
        if (v[i] == val) {
            index = i;
            break;
        }
    }
#else
    auto iter = std::find(std::begin(v), std::end(v), val);
#endif
}

// P2. Write in ISO Standard C++

// P3. Express Intent

struct Point
{
    int x;
    int y;
};

void drawLine(const Point& p0, const Point& p1)
{
    (void)p0;
    (void)p1;
}

// P4. Ideally, a program should be statically type safe
//      unions -> use variant(C++17)
//      casts -> minimize their use; templates can help
//      array decay -> use span
//      range errors -> use span
//      narrowing conversions -> minimize their use and use narrow or narrow_cast

// P5. Prefer compile-time checking to run-time checking

void read(int* p, int n)
{
    for (int i=0; i<n; i++) {
        std::cout << p[i] << " ";
    }
    std::cout <<  std::endl;
}

template<typename T>
void read(std::span<T> a)
{
    std::copy(std::begin(a), std::end(a), std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::endl;
}

} // namespace ================================================================

void test3()
{
    auto v = std::vector<int>{1, 2, 3};
#if (0)
    auto i = 0;
    while (i < v.size()) {
        i++;
    }
#else
    for (const auto& x : v) {

    }
#endif

    drawLine(Point{0, 0}, Point{10, 10});
}

void test4()
{
    int array[] = {1, 2, 3, 4, 5};
    auto a = std::span{array};
    for (const auto& i: a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void test5()
{
    using Int = int;

#if (0)
    int bits = 0;
    for (Int i=1; i; i <<= 1)
        ++bits;

    if (bits < 32)
        std::cerr << "Int too small" << std::endl;
#else
    static_assert (sizeof(Int) >= 4);
#endif

    int array[] = { 1, 2, 3, 4, 5 };

    std::cout << "Bad." << std::endl;
    read(array, 5);

    std::cout << "Good." << std::endl;
    read<int>(array);
}
