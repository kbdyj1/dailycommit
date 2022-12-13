#include <iostream>
#include <tuple>
#include <sstream>

namespace { //=================================================================

namespace _1 {

class Number {
public:
    static int zero() {
        return 0;
    }
    static int increment(const int value) {
        return value + 1;
    }
};

const std::tuple<int,int> incrementAllPure(const int& first, const int& second)
{
    return std::make_tuple(first + 1, second + 1);
}

void test()
{
    auto results = incrementAllPure(1, 2);
    std::cout << std::get<0>(results) << ", " << std::get<1>(results) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 {

const int* incrementPointerAddressAndValuePure(const int* const& p)
{
    return p + 1;
}

std::string formatResultsPure(const int* p)
{
    std::stringstream out;

    out << "address: " << p << "\n";
    out << "inc pointer address and value pure: " << incrementPointerAddressAndValuePure(p) << "\n";
    out << "address: " << p << "\n";
    out << "value: " << *p << "\n";

    return out.str();
}

void test()
{
    int* p = new int(10);

    auto s = formatResultsPure(p);

    std::cout << s;

    delete p;
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_02()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
