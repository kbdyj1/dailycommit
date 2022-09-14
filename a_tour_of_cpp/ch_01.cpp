//=============================================================================
//  A tour of C++
//  Ch.01  Foundation
//
//  22/09/12
//=============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace { //=================================================================

void test_if(const std::vector<int>& v)
{
#if 201703 <= __cplusplus
    if (auto n = v.size(); n != 0) {
#else
    auto n = v.size();
    if (n != 0) {
#endif
        auto sum = std::accumulate(std::begin(v), std::end(v), 0);
        std::cout << "sum: " << sum << std::endl;
    }
}

} //namespace =================================================================

void test_ch_01()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    test_if(v);
}
