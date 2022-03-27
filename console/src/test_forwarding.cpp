#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace {

void f(std::vector<int> param)
{
    std::copy(param.begin(), param.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout<< std::endl;
}
void f(int value)
{
    std::cout << "f(" << value << ")" << std::endl;
}
void f(int pf(int))
{
    auto ret = pf(0);
    std::cout << "pf(0) : " << ret << std::endl;
}

template <typename T>
void forward(T&& param)
{
    f(std::forward<T>(param));
}

void test0()
{
#if (0)
    //22:12: error: no matching function for call to ‘forward(<brace-enclosed initializer list>)’
    //   22 |     forward({1, 2, 3});
    //      |     ~~~~~~~^~~~~~~~~~~
    forward({1, 2, 3});
#else
    f({1, 2, 3});

    auto param = {4, 5, 6};
    forward(param);
#endif
}

class Widget
{
public:
    static constexpr std::size_t kMinVal = 4;
};

void test1()
{
    auto wl = std::vector<Widget>{};
    wl.reserve(Widget::kMinVal);

    f(Widget::kMinVal);

#if (0)
    // some compilers require definitions
    // g++ OK
    forward(Widget::kMinVal);
#endif
}

int processVal(int a)
{
    return a*a;
}
int processVal(int a, int priority)
{
    return a*a + priority;
}

void test2()
{
    f(processVal);
#if (0)
    //77:12: error: no matching function for call to ‘forward(<unresolved overloaded function type>)’
    //   77 |     forward(processVal);
    //      |     ~~~~~~~^~~~~~~~~~~~
    forward(processVal);
#else
    using Func = int(*)(int);

    forward(static_cast<Func>(processVal));

    Func fptr = processVal;
    forward(fptr);
#endif
}

struct IP4 {
    std::uint32_t
        version: 4,
        IHL: 4,
        DSCP: 6,
        ECN: 2,
        len: 16;
};

void test3()
{
    IP4 ip;
    f(ip.version);

#if (0)
    //105:16: error: cannot bind bit-field ‘ip.{anonymous}::IP4::version’ to ‘unsigned int&’
    //  105 |     forward(ip.version);
    //      |             ~~~^~~~~~~
    forward(ip.version);
#endif
}

} // namespace ================================================================

void test_forwarding()
{
    //test0();
    //test1();
    test2();
}
