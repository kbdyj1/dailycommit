#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace { //=================================================================

namespace _1 {

int x2(int value)
{
    return value * 2;
}

void test()
{
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    std::vector<int> v1;

    std::transform(v0.begin(), v0.end(), std::back_inserter(v1), x2);

    std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_container_size()
{
    _1::test();
}
