#include <iostream>
#include <vector>
#include <algorithm>

namespace { //=================================================================

auto vInt = std::vector<int>{ 1, 10, 100, 30, 50, 8, 95, 20, 48, 25 };

namespace _1 {

class GreaterThan {
    int value;

public:
    GreaterThan(int value) : value(value)
    {}
    bool operator()(int arg) const {
        return arg > value;
    }
};

void test()
{
    auto iter = std::partition(vInt.begin(), vInt.end(), GreaterThan(30));

    for (auto i : vInt) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "distance: " << std::distance(vInt.begin(), iter) << "\n";
    for ( ; iter != vInt.end(); iter++) {
        std::cout << *iter << " ";
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename Function, typename SecondArg>
class Bind2nd {
    Function func;
    SecondArg secondArg;

public:
    Bind2nd(Function func, SecondArg arg) : func(func), secondArg(arg)
    {}

    template<typename FirstArg>
    auto operator()(FirstArg&& firstArg) const -> decltype(func(std::forward<FirstArg>(firstArg), secondArg)) {
        return func(std::forward<FirstArg>(firstArg), secondArg);
    }
};

bool greater_than(int l, int r)
{
    return l > r;
}

void test()
{
    Bind2nd greater_than_30(greater_than, 30);

    auto iter = std::partition(vInt.begin(), vInt.end(), greater_than_30);

    for (auto i : vInt) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "distance: " << std::distance(vInt.begin(), iter) << "\n";
    for ( ; iter != vInt.end(); iter++) {
        std::cout << *iter << " ";
    }
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_ch_04()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
