//=============================================================================
//  A tour of C++
//  Ch.12  Algorithm
//
//  22/09/17
//=============================================================================

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

namespace { //=================================================================

void test_unique_copy()
{
    auto v = std::vector<int>{ 1, 1, 2, 3, 4, 4, 5, 4, 6, 3 };
    std::sort(std::begin(v), std::end(v));

#if (0)
    auto l = std::list<int>(v.size());
    std::unique_copy(std::begin(v), std::end(v), std::begin(l));
#else
    auto l = std::list<int>{};
    std::unique_copy(std::begin(v), std::end(v), std::back_inserter(l));
#endif

    std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, " "));
}

void test_copy_if()
{
    auto v0 = std::vector<int>{ 1, 1, 2, 3, 4, 4, 5, 4, 6, 3 };
    auto v1 = std::vector<int>{};

    std::copy_if(std::begin(v0), std::end(v0), std::back_inserter(v1), [](int value){
        return value % 2 == 0;
    });
    for (auto i : v1) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::sort(std::begin(v0), std::end(v0));

    auto [p0, p1] = std::equal_range(std::begin(v0), std::end(v0), 4);
    std::cout << "p0: " << std::distance(std::begin(v0), p0) << "\n";
    std::cout << "p1: " << std::distance(std::begin(v0), p1) << "\n";
    std::cout << std::endl;
}

} //namespace =================================================================

void test_ch_12()
{
#if (0)
    test_unique_copy();
#endif

    test_copy_if();
}
