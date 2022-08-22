#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

namespace { //=================================================================

using namespace std::literals;

class L {
    int b;
    int e;
public:
    explicit L(int b, int e) : b(b), e(e)
    {}
    L(const L&) = default;
    L(L&&) = default;
    L& operator=(const L&) = delete;
    ~L() = default;

    bool operator()(const int n) const {
        return b <= n && n < e;
    }
};

void test0()
{
    auto b = 4;
    auto e = 7;
    auto v = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#if (1)
    auto l = [b, e](const int n) { return b <= n && n < e; };
#else
    auto l = L(b, e);
#endif
    auto n = std::count_if(std::begin(v), std::end(v), l);
    std::cout << b << "<= && " << e << " <: " << n << "\n";
}

void test_generic_lambda()
{
    auto v = std::vector<int>{0, 2, 4, -3, 5, -1, 6, -4, 8, 7};
    auto sum = [](const auto a, const auto b) {
        return a + b;
    };
    auto result = std::accumulate(std::begin(v), std::end(v), 0, sum);
    std::cout << "sum: " << result << "\n";

    auto sv = std::vector<std::string>{"Hello, ", "Qt ", "6 ", "!!!"};
    auto sresult = std::accumulate(std::begin(sv), std::end(sv), ""s, sum);
    std::cout << "sum: " << sresult << "\n";
}

std::function<int(const int)> makeFib()
{
    std::function<int(const int)> f = [](const int n) {
        std::function<int(const int)> fib = [&fib](int n) {
            return n <= 2 ? 1 : fib(n-1) + fib(n-2);
        };
        return fib(n);
    };
    return f;
}

std::function<int(const int)> badMakeFib()
{
    std::function<int(const int)> fib = [&fib](const int n) {
        return n <= 2 ? 1 : fib(n-1) + fib(n-2);
    };
    return fib;
}

void test_recursive_lambda()
{
#if (0)
    std::function<int(const int)> fib = [&fib](const int n) {
        return n <= 2 ? 1 : fib(n-1) + fib(n-2);
    };
#else
    //auto fib = makeFib();
    auto fib = badMakeFib();
#endif
    const int n = 10;
    auto result = fib(n);

    std::cout << "fib(" << n << "): " << result << "\n";
}

} //namespace =================================================================

void test_ch_03_lambda()
{
    //test0();
    //test_generic_lambda();
    test_recursive_lambda();
}
