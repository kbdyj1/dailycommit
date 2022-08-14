#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <atomic>

namespace { //=================================================================

using namespace std::literals;

template <typename F, typename T>
auto apply(F&& f, T value)
{
    return f(value);
}

class Widget {
    int mValue;
public:
    Widget(int value = 0) : mValue(value)
    {}

    int& value() {
        return mValue;
    }
};

// C++11
auto func1(int const i) -> int {
    return 2 * i;
}
//C++14~
auto func2(int const i) {
    return 2 * i;
}

void test_auto()
{
    auto i = 40;
    auto d = 40.0;
    auto s = "40.0";
    auto v = { 1, 2, 3, 4, 5 };

    std::cout << typeid(i).name() << "\n";
    std::cout << typeid(d).name() << "\n";
    std::cout << typeid(s).name() << "\n";
    std::cout << typeid(v).name() << "\n";

    std::cout << "\n";

    auto add = [](auto const a, auto const b) {
        return a + b;
    };

    auto iSum = add(1, 40);
    auto dSum = add(1.0, 40.0);
    auto sSum = add("Hello, "s, "Qt6"s);

    std::cout << typeid(iSum).name() << "\n";
    std::cout << typeid(dSum).name() << "\n";
    std::cout << typeid(sSum).name() << ", sSum: " << sSum << "\n";

    auto result = apply(sqrt, 4.0);
    std::cout << "apply(sqrt, 4.0): " << result << ", " << typeid(result).name() << "\n";

    auto vec = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto size = int( vec.size() );

    Widget w(10);
#if (0)
    auto value = w.value();
#else
    auto& value = w.value();
#endif
    value = 20;
    std::cout << "value: " << w.value() << "\n";

    auto aValue = std::atomic<int>(10); // ???

    typedef long long llong;

//    auto l0 = long long{ 10 };    // compile-error
    auto l1 = llong{ 20 };
    auto l2 = 30LL;
}

} //namespace =================================================================

void test_ch_01()
{
    test_auto();
}
