#include <iostream>
#include <functional>
#include <iterator>

namespace { //=================================================================

namespace _1 {

void freeFunc()
{
    std::cout << "Hello, Qt6\n";
}

class A
{
public:
    int member() const
    {
        return 1;
    }
};

void test()
{
    auto f = std::function<void()>(freeFunc);

    f();

    auto mf = std::function<int(const A&)>(&A::member);
    auto a = A{};

    std::cout << "A::member(): " << mf(a) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

// ariithmetic : plus, minus, multiplies, divides, modules, negate
// comparision : equal_to, not_equal_to, greater, less, greater_equal, less_equal
// logical_and, logical_or, logical_not
// bit_and, bit_or, bit_xor

void test()
{
    auto v = std::vector<int>{ 3, 8, 4, 1, 9, 16, 2, 20 };

    std::sort(v.begin(), v.end(), std::greater<int>());

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

} //_2 --------------------------------------------------------------

namespace _3 {

auto add = [](int l, int r) {
    std::cout << l << " + " << r << "\n";
    return l + r;
};

void test()
{
    auto increment = std::bind(add, 1, std::placeholders::_1);//, 1);

    std::cout << "increment(2) : " << increment(2) << "\n";
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15()
{
#if (0)
    _1::test();
    _2::test();
#endif

    _3::test();
}
