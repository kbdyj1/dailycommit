#include <functional>
#include <iostream>
#include <type_traits>
#include <cmath>
//#include <source_location>
#include <string_view>

namespace { //=================================================================

int plusFunc(int a, int b)
{
    return a + b;
}
auto plusLambda = [](int a, int b) {
    return a + b;
};

void test1()
{
#if (0)
    auto a = std::bind_front(plusFunc, 2000);
    std::cout << "a(20) : " << a(20) << std::endl;

    auto b = std::bind_front(plusLambda, 2000);
    std::cout << "b(20) : " << b(20) << std::endl;

    auto c = std::bind_front(std::plus<int>(), 2000);
    std::cout << "c(20) : " << c(20) << std::endl;

    using namespace std::placeholders;

    auto d = std::bind(plusFunc, 2000, _1);
    std::cout << "d(20) : " << d(20) << std::endl;

    auto e = [](int i) { return plusLambda(2000, i); };
    std::cout << "e(20) : " << e(20) << std::endl;
#endif
}

constexpr double power(double b, int x)
{
    if (std::is_constant_evaluated() && !(b == 0.0 && x < 0)) {
        if (0 == x)
            return 1.0;

        double r = 1.0, p = x > 0 ? b : 1.0 / b;
        auto u = unsigned(x > 0 ? x : -x);
        while (0 != u) {
            if (u & 1)
                r *= p;
            u /= 2;
            p *= p;
        }
        return r;
    } else {
        return std::pow(b, double(x));
    }
}

void test2()
{
    constexpr double k = power(10.0, 3);
    std::cout << "kilo1 : " << k << std::endl;

    int n = 3;
    double k2 = power(10.0, n);
    std::cout << "kilo2 : " << k2 << std::endl;
}

} // namespace ================================================================

void test_ch4_7()
{
    std::cout << std::endl;
    std::cout << std::boolalpha;

    //test1();
    test2();
}
