#include <iostream>
#include <functional>

namespace  {

double divMe(double a, double b)
{
    return a / b;
}
namespace _1 { //use Bind

using namespace std::placeholders;

void test()
{
    std::cout << "\n";

    std::cout << "1/2.0 = " << std::bind(divMe, 1, 2.0)() << "\n";

    auto myDivBindPlaceholder = std::bind(divMe, std::placeholders::_1, std::placeholders::_2);
    std::cout << "1/2.0 = " << myDivBindPlaceholder(1, 2.0) << "\n";

    auto myDivBindPlaceholderSwap = std::bind(divMe, std::placeholders::_2, std::placeholders::_1);
    std::cout << "1/2.0 = " << myDivBindPlaceholderSwap(2.0, 1) << "\n";

    auto myDivBind1st = std::bind(divMe, std::placeholders::_1, 2.0);
    std::cout << "1/2.0 = " << myDivBind1st(1) << "\n";

    auto myDivBind2nd = std::bind(divMe, 1, std::placeholders::_1);
    std::cout << "1/2.0 = " << myDivBind2nd(2.0) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 { //use Lambda

void test()
{
    std::cout << "\n";

    std::cout << "1/2.0 = " << [](double a, double b){ return divMe(a, b); }(1, 2.0) << "\n";

    auto myDivBindPlaceholder = [](double a, double b){ return divMe(a, b); };
    std::cout << "1/2.0 = " << myDivBindPlaceholder(1, 2.0) << "\n";

    auto myDivBindPlaceholderSwap = [](double a, double b){ return divMe(b, a); };
    std::cout << "1/2.0 = " << myDivBindPlaceholderSwap(2.0, 1) << "\n";

    auto myDivBind1st = [](double a){ return divMe(a, 2.0); };
    std::cout << "1/2.0 = " << myDivBind1st(1) << "\n";

    auto myDivBind2nd = [](double a){ return divMe(1, a); };
    std::cout << "1/2.0 = " << myDivBind2nd(2.0) << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

int plusFunc(int a, int b)
{
    return a + b;
}
auto plusLambda = [](int a, int b)
{
    return a + b;
};

void test()
{
    std::cout << "\n";

    auto twoThousandPlus1 = std::bind_front(plusFunc, 2000);
    std::cout << "twoThousandPlus1(20): " << twoThousandPlus1(20) << '\n';

    auto twoThousandPlus2 = std::bind_front(plusLambda, 2000);
    std::cout << "twoThousandPlus2(20): " << twoThousandPlus2(20) << '\n';

    auto twoThousandPlus3 = std::bind_front(std::plus<int>(), 2000);
    std::cout << "twoThousandPlus3(20): " << twoThousandPlus3(20) << '\n';

    auto twoThousandPlus4 = std::bind(plusFunc, 2000, std::placeholders::_1);
}

} //_3 --------------------------------------------------------------

} //===========================================================================

void test_partial_function()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
