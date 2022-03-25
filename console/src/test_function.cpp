#include <functional>
#include <iostream>

namespace { //=================================================================

//-----------------------------------------------------------------------------
//  std::function satisfies the requirements of CopyConstructable & CopyAssignable
//-----------------------------------------------------------------------------

void test0()
{
    auto f = std::function<void()>{};

    try {
        std::cout << f.operator bool() << std::endl;
        f();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl; // bad_function_call
    }
}

struct A
{
    A(int num) : mNum(num)
    {}
    void printAdd(int i) const { std::cout << mNum+1 << std::endl; }
    int mNum;
};

void printNum(int i)
{
    std::cout << i << std::endl;
}

struct PrintNum
{
    void operator()(int i) const
    {
        std::cout << i << std::endl;
    }
};

void test1()
{
    auto f_display = std::function<void(int)>{printNum};
    f_display(9);

    auto f_display2 = std::function<void()>{ [](){ printNum(2); }};
    f_display2();

    auto f_display3 = std::function<void()>{ std::bind(printNum, 3) };
    f_display3();

    auto f_add_display = std::function<void(const A&, int)>{&A::printAdd};
    const A a{4};
    f_add_display(a, 1);
    f_add_display(5, 1);
}

} // namespace ================================================================

void test_functional()
{
    //test0();
    test1();
}
