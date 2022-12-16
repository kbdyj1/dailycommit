#include <iostream>
#include <functional>

namespace { //=================================================================

auto add = [](const int first, const int second) {
    return first + second;
};

namespace _1 {

#if (0)
auto increment = [](const int second) {
    return add(second, 1);
};
#else
auto increment = std::bind(add, 1, std::placeholders::_1);
#endif

auto add3 = [](const int first, const int second, const int third) {
    return first + second + third;
};

void test()
{
    std::cout << "increment(10): " << increment(10) << "\n";

    auto addTowNumbersTo = std::bind(add3, 10, std::placeholders::_1, std::placeholders::_2);

    std::cout << addTowNumbersTo(20, 12) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

class Add {
    int first;
    int second;

public:
    Add(int first, int second) : first(first), second(second)
    {}

    int add() const {
        return first + second;
    }
};

class AddTo {
    int first;

public:
    AddTo(int first) : first(first)
    {}

    int addTo(int second) const {
        return first + second;
    }
};

void test()
{
    auto op = Add(10, 20);

    auto addOp = std::bind(&Add::add, op);

    std::cout << addOp() << "\n";

    auto addOp2 = std::bind(&Add::add, std::placeholders::_1);

    std::cout << addOp2(op) << "\n";

    auto op2 = AddTo(10);

    auto addToOp = std::bind(&AddTo::addTo, op2, std::placeholders::_1);

    std::cout << addToOp(10) << "\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

auto curryAdd = [](const int first) {
    return [=](const int second) {
        return first + second;
    };
};

auto curryAdd3 = [](const int first) {
    return [first](const int second) {
        return [first, second](const int third) {
            return first + second + third;
        };
    };
};

void test() {
    std::cout << curryAdd(10)(20) << "\n";

    auto increment = curryAdd(1);

    std::cout << "increment(2): " << increment(2) << "\n";

    auto curryAddPartialApplication = [](const int first) {
        return std::bind(add, first, std::placeholders::_1);
    };

    std::cout << "curryAddPartialApplication(14)(14): " << curryAddPartialApplication(14)(14) << "\n";

    std::cout << "curryAdd3(1)(2)(3): " << curryAdd3(1)(2)(3) << "\n";
}

} //_3 --------------------------------------------------------------

namespace _4 {

auto simple1 = [](auto f) {
    return f;
};
auto simple2 = [](auto f) {
    return [f](auto x) {
        return std::bind(f, x, std::placeholders::_1);
    };
};
auto simple3 = [](auto f) {
    return [f](auto x, auto y) {
        return std::bind(f, x, y, std::placeholders::_1);
    };
};
auto simple4 = [](auto f) {
    return [f](auto x, auto y, auto z) {
        return std::bind(f, x, y, z, std::placeholders::_1);
    };
};

auto curry2 = [](auto f) {
    return simple2(f);
};
auto curry3 = [](auto f) {
    return curry2(simple3(f));
};
auto curry4 = [](auto f) {
    return curry3(simple4(f));
};

void test()
{
    auto add = [](int first, int second) {
        return first + second;
    };
    std::cout << curry2(add)(1)(2) << "\n";


    auto add3 = [](int first, int second, int third) {
        return first + second + third;
    };
    std::cout << curry3(add3)(1)(2)(3) << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

auto addWrapped = [](const auto first, const auto second) {
    return (first + second) % 20;
};

template <typename T, T one>
auto incrementWrapped = std::bind(addWrapped, std::placeholders::_1, one);

void test()
{
    std::cout << "incrementWrapped<int, 1>(20): " << incrementWrapped<int, 1>(20) << "\n";
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_05()
{
#if (0) //done
    _1::test();
    _2::test();
    _3::test();
    _4::test();
#endif

    _5::test();
}
