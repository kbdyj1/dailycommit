#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <numeric>

namespace { //=================================================================

namespace _1 {

class Calculator {
    int first;
    int second;

public:
    Calculator(int first, int second) : first(first), second(second)
    {}

    int add() const {
        return first + second;
    }
    int multiply() const {
        return first * second;
    }
    int mod() const {
        return first % second;
    }
    int negateInt() const {
        return -first;
    }
};

auto add = [](auto first, auto second) {
    return first + second;
};
auto multiply = [](auto first, auto second) {
    return first * second;
};
auto mod = [](auto first, auto second) {
    return first % second;
};
auto negate = [](const auto value) {
    return -value;
};

auto calculatorMap = [](const auto first, const auto second) -> std::map<std::string, std::function<int()>> {
    return {
        { "add", std::bind(add, first, second) },
        { "multiply", std::bind(multiply, first, second) },
        { "mod", std::bind(mod, first, second) },
        { "negate", std::bind(negate, first) }
    };
};

void test() {
    const auto first = 1;
    const auto second = 2;
    auto calculator = calculatorMap(first, second);

    std::cout << "add(" << first << ", " << second << "): " << calculator["add"]() << "\n";
    std::cout << "multiply(" << first << ", " << second << "): " << calculator["multiply"]() << "\n";
    std::cout << "mod(" << first << ", " << second << "): " << calculator["mod"]() << "\n";
    std::cout << "negate(" << first << ", " << second << "): " << calculator["negate"]() << "\n";
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_08()
{
    _1::test();
}
