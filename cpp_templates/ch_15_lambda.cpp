#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

namespace { //=================================================================

namespace _1 {

template <typename Iter>
Iter findNegative(Iter b, Iter e)
{
#if __cplusplus < 201402L
    return std::find_if(b, e, [](typename std::iterator_traits<Iter>::value_type value){
        return value < 0;
    });
#else
    return std::find_if(b, e, [](auto value) {
        return value < 0;
    });
#endif
}

void test()
{
    auto v = std::vector<int>{ 1, 2, -3, 4, 5 };
    auto iter = findNegative(v.begin(), v.end());
    if (iter != v.end()) {
        std::cout << "-? " << *iter;
    }
}
} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_lambda()
{
    _1::test();
}
