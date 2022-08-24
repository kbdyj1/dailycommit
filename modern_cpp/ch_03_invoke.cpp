#include <iostream>
#include <functional>

namespace { //=================================================================

int add(const int a, const int b)
{
    return a + b;
}
struct F {
    int x = 0;
    void inc(const int n) {
        std::cout << "inc(" << n << ")\n";
        x += n;
    }
};

void test()
{
    auto a0 = std::invoke(add, 1, 2);
    std::cout << "std::invoke(add, 1, 2): " << a0 << "\n";

    int(*func)(int,int) = add;
    auto a1 = std::invoke(func, 1, 2);
    std::cout << "std::invoke(func, 1, 2): " << a1 << "\n";

    F f;
    std::invoke(&F::inc, f, 10);

    auto a2 = std::invoke(&F::x, f);
    std::cout << "std::invoke(&F::x, f): " << a2 << "\n";

    auto a3 = std::invoke(std::plus<>(), std::invoke(&F::x, f), 20);
    std::cout << "std::invoke(std::plus<>(), std::invoke(&F::x, f), 20): " << a3 << "\n";

    auto l = [](auto a, auto b) { return a * b; };
    auto a4 = std::invoke(l, 3, 4);
    std::cout << "std::invoke(l, 3, 4): " << a4 << "\n";

    void (F::*mfunc)(int) = &F::inc;
    (f.*mfunc)(15);
}

} //namespace =================================================================

void test_ch_03_invoke()
{
    test();
}
