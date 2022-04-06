#include <iostream>
#include <vector>
#include <memory>

namespace { //=================================================================

// F1. "Package" meaningful operations as carefully named function

// Bad
void readAndPrint(std::istream& is)
{
    int x;
    if (is >> x)
        std::cout << "the int is " << x << std::endl;
    else
        std::cerr << "no int on input" << std::endl;
}

// F2. A function should perform a single logical operation

#define USING_AUTO

#ifndef USING_AUTO
int read(std::istream& is)
{
    int x;
    is >> x;

    return x;
}

void print(std::ostream& os, int x)
{
    os << x << std::endl;
}
#else
auto read = [](auto& in, auto& value)
{
    in >> value;
};

auto print(auto& out, const auto& value)
{
    out << value << std::endl;
}
#endif

void readAndPrintGood()
{
#ifndef USING_AUTO
    auto x = read(std::cin);
#else
    int x;
    read(std::cin, x);
#endif
    print(std::cout, x);
}

// F3. Keep functions short and simple

// F4. If a function might have to be evaluated at compile time, declare it constexpr

constexpr int factorial(int n)
{
    constexpr int MaxExp = 17;
    int x = 1;
    for (auto i=2; i <= n; i++)
        x *= i;
    return x;
}

constexpr int cMin(int x, int y) { return x < y ? x : y; }

// F5. If a function is very small and time-critical, declare it inline

// F6. If your function must not throw, declare it noexcept

std::vector<std::string> collect(std::istream& in) noexcept
{
    std::vector<std::string> res;

    for (std::string s; in >> s; ) {
        res.push_back(s);
    }
    return res;
}

// F7. For general use, take T* or T& arguments rather than smart pointers

class Widget
{};

void use(const Widget& w)
{
    (void)w;
}
void fBad(std::shared_ptr<Widget>& w)
{
    std::cout << "fBad(std::shared_ptr<Widget>&)" << std::endl;
    use(*w);
}

void fGood(const Widget& w)
{
    std::cout << "fBad(Widget&)" << std::endl;
    use(w);
}

// F8. Prefer pure functions

template <typename T>
auto square(T t)
{
    return t * t;
}

// F9. Unused parameters should be unnamed

} // namespace ================================================================

void test_f_1()
{
    readAndPrint(std::cin);
}

void test_f_4()
{
    std::cout << "factorial(8) : " << factorial(8) << std::endl;
    std::cout << "cMin(5, 10) : " << cMin(5, 10) << std::endl;
}

void test_f_7()
{
    std::shared_ptr<Widget> w0;
    fBad(w0);

    Widget w1;
    //fBad(w1);

    fGood(*w0);
    fGood(w1);
}
