#include <iostream>
#include <functional>

namespace { //=================================================================

namespace _1 {

std::function<int(int)> compose(std::function<int(int)> f, std::function<int(int)> g)
{
    return [f, g](auto value) {
        return f(g(value));
    };
}

void test() {
    auto increment = [](const int value) {
        return value + 1;
    };
    auto incrementTwice = [increment](int value) {
        return increment(increment(value));
    };

    auto result = incrementTwice(1);

    std::cout << "result == 3 ? " << result << "\n";

    auto square = [](int value) {
        return value * value;
    };
    auto incrementSquare = [=](int value) {
        return increment(square(value));
    };

    std::cout << "incrementSquare(3): " << incrementSquare(3) << "\n";

    std::cout << "compose(increment, square)(3): " << compose(increment, square)(3) << "\n";
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_04()
{
    _1::test();
}
