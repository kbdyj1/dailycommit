#include <iostream>
#include <vector>

namespace { //=================================================================

namespace _1 {

double max(const std::vector<double>& numbers)
{
    assert(!numbers.empty());
    auto result = std::max_element(numbers.cbegin(), numbers.cend());
    return *result;
}

void test()
{
    auto sum = max({1}) + max({1, 2}) + max({1, 2, 3});
    std::cout << "sum: " << sum << "\n";
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_ch_05()
{
    _1::test();
}
