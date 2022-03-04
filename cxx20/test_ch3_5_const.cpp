#include <iostream>

extern constinit int staticA;

auto staticB = staticA;

namespace //===================================================================
{

consteval int sqr(int n) {
    return n * n;
}
int sqrRunTime(int n) {
    return n * n;
}
consteval int sqrCompileTime(int n) {
    return n * n;
}
constexpr int sqrRunOrCompileTime(int n) {
    return n * n;
}

constexpr auto res1 = sqr(5);
constinit auto res2 = sqr(6);

void test1()
{
    std::cout << "sqr(5) : " << sqr(5) << std::endl;

    const int a = 4;
    std::cout << "sqr(a) : " << sqr(a) << std::endl;

    int b = 3;
    //std::cout << "sqr(b) : " << sqr(b) << std::endl;
}

void test2()
{
    std::cout << "sqr(5) : " << res1 << std::endl;
    std::cout << "sqr(6) : " << res2 << std::endl;

    constinit thread_local auto res3 = sqr(7);

    std::cout << "sqr(7) : " << res3 << std::endl;
}

void test3()
{
    //error: constexpr variable 'n0' must be initialized by a constant expression
    //constexpr auto n0 = sqrRunTime(10);

    constexpr auto n1 = sqrCompileTime(10);
    constexpr auto n2 = sqrRunOrCompileTime(20);

    int x = 15;

    auto n3 = sqrRunTime(x);
    //auto n4 = sqrCompileTime(x);
}

constexpr int constExprValue = 1000;
constinit int constInitValue = 1100;

int inc4(int value) { return ++value; }

void test4()
{
    auto val = 1000;
    const auto res = inc4(val);
    std::cout << "res: " << res << std::endl;

    //++res;
    //++constExprValue;
    std::cout << "++constInitValue: " << ++constInitValue << std::endl;

    constexpr auto localConstExpr = 2000;
    //constinit auto localConstInit = localConstExpr;

    std::cout << "staticB: " << staticB << std::endl;
}

} // namespace ================================================================

void test_ch3_5_const()
{
    std::cout << std::endl;

    //test1();
    //test2();
    test4();
}
