#include <iostream>
#include <vector>
#include <algorithm>

namespace { //=================================================================

bool isPrime(unsigned long n)
{
    if (n <= 1)
        return false;
    for (auto i=2; i<n; ++i)
        if (n % i == 0)
            return false;

    return true;
}

void test_find()
{
    auto v0 = std::vector<int>{1, 1, 2, 3, 5, 8, 13 };
    auto iter = std::find(std::begin(v0), std::end(v0), 3);
    if (iter != v0.end()) {
        std::cout << *iter << "\n";
    }

    iter = std::find_if(std::begin(v0), std::end(v0), [](const int n){
        return 10 <= n;
    });
    if (iter != v0.end()) {
        std::cout << *iter << "\n";
    }

    auto sub = std::vector<int>{ 6, 8, 11 };
    iter = std::find_first_of(std::begin(v0), std::end(v0), std::begin(sub), std::end(sub));
    if (iter != v0.end()) {
        std::cout << *iter << "\n";
    }

    auto v1 = std::vector<int>{ 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1 };
    auto v2 = std::vector<int>{ 1, 0, 1 };

#if (0)
    iter = std::find_end(v1.begin(), v1.end(), v2.begin(), v2.end());
#else
    iter = std::search(v1.begin(), v1.end(), v2.begin(), v2.end());
#endif

    std::cout << "search() index: " << std::distance(v1.begin(), iter) << "\n";

    iter = std::search_n(v1.begin(), v1.end(), 2, 0);

    std::cout << "search_n(2, 0) index: " << std::distance(v1.begin(), iter) << "\n";

    iter = std::adjacent_find(v0.begin(), v0.end());
    std::cout << "adjacent_find() index: " << std::distance(v0.begin(), iter) << "\n";

    iter = std::adjacent_find(v0.begin(), v0.end(), [](const int a, const int b){
        return isPrime(a) && isPrime(b);
    });
    std::cout << "adjacent_find( isPrime ) index: " << std::distance(v0.begin(), iter) << "\n";
}

} //namespace =================================================================

void test_ch_05_algorithm()
{
    test_find();
}
