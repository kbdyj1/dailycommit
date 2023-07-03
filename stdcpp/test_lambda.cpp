#include <iostream>
#include <vector>
#include <functional>

namespace { //=================================================================

namespace _1 {

void test()
{
    int x = 10;
    auto addX = [&x](int y) { return x + y; };
    std::cout << "addX(5): " << addX(5) << "\n";

    x = 20;
    auto l0 = addX;
    std::cout << "l0(5): " << l0(5) << "\n";

    x = 30;
    auto l1 = addX;
    std::cout << "l1(5): " << l1(5) << "\n";

}

} //_1 --------------------------------------------------------------

namespace _2 {

using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;

int compute0()
{
    return 3;
}
int compute1()
{
    return 5;
}
int computeDivisor(int a, int b)
{
    return a % b;
}

void addDivisorFilter()
{
    auto c0 = compute0();
    auto c1 = compute1();
    auto divisor = computeDivisor(c0, c1);

    filters.emplace_back([&](int value) -> int {
        return value % divisor; }
    );
    auto f0 = filters.at(0);
    auto f1 = [&](int value) -> int {
        return value % divisor; };

    std::cout << f0(5) << std::endl;
    std::cout << f1(5) << std::endl;
    std::cout << "5 % 3 = " << (5 % 3) << std::endl;
}

void test()
{
    addDivisorFilter();
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_lambda()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
