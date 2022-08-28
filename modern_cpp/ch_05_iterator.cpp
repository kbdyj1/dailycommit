#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

#include "util.h"
#include "MyArray.h"

namespace { //=================================================================

void test_inserter()
{
    auto l0 = std::list<int>{ 1, 2, 3, 4, 5 };
    auto l1 = std::list<int>{ 6, 7, 8 };
    std::copy(l1.begin(), l1.end(), std::front_inserter(l0));
    print(l0);

    auto v0 = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto it = v0.begin();
    std::advance(it, 3);
    std::fill_n(std::inserter(v0, it), 3, 0);
    print(v0);
}

void test_my_array()
{
    auto a = MyArray<int, 3>{};
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    print(a);

    std::transform(a.begin(), a.end(), a.begin(), [](const int n){
        return n*2;
    });
    std::cout << "transform(...): ";
    print(a);

    std::fill_n(a.begin(), 3, 1);
    std::cout << "fill_n(..., 1): ";
    print(a);

    std::cout << "for(auto e: a)\n";
    for (auto&& e : a) {
        std::cout << e << " ";
    }
    std::cout << "\n";
}

void test_non_member_std_functions()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };

    std::cout << std::boolalpha;

    PRINT_FUNC(std::size(v));
    PRINT_FUNC(std::empty(v));
    PRINT_FUNC(std::data(v));

    int array[5] = { 1, 2, 3, 4, 5 };

    PRINT_FUNC(std::size(array));
}

template <typename F, typename C>
void process(F&& func, const C& c)
{
    std::for_each(std::begin(c), std::end(c), std::forward<F>(func));
}

void test_process()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    auto f = [](const auto e){ std::cout << e << " "; };
    process(f, v);
    std::cout << "\n";

    int a[5] = { 2, 4, 6, 8, 10 };
    process(f, a);
    std::cout << "\n";
}

} //namespace =================================================================

void test_ch_05_iterator()
{
#if (0) // done
    test_inserter();
    test_my_array();
#endif

    test_non_member_std_functions();
    test_process();
}
