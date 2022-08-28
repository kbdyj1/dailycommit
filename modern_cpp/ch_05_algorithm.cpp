#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>

#include "util.h"

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

//    upper_bound(..., 1): 2
//    upper_bound(..., 2): 3
//    upper_bound(..., 3): 5
    for (auto i=1; i<10; i++) {
        iter = std::upper_bound(v0.begin(), v0.end(), i);
        if (iter != v0.end()) {
            std::cout << "upper_bound(..., " << i << "): " << *iter << "\n";
        }
    }

//    lower_bound(..., 3): 3
//    lower_bound(..., 4): 5
//    lower_bound(..., 5): 5
    std::cout << "\n";
    for (auto i=1; i<10; i++) {
        iter = std::lower_bound(v0.begin(), v0.end(), i);
        if (iter != v0.end()) {
            std::cout << "lower_bound(..., " << i << "): " << *iter << "\n";
        }
    }

    // first: lower_bound
    // second: upper_bound
    auto bounds = std::equal_range(v0.begin(), v0.end(), 1);
    std::cout << "first: " << std::distance(v0.begin(), bounds.first)
              << ", second: " << std::distance(v0.begin(), bounds.second) << "\n";
}

void test_sort()
{
    auto v0 = std::vector<int>{ 5, 19, 8, 4, 21, 7, 10, 6, 3, 15 };
#if (0)
    std::sort(v0.begin(), v0.end());
#else
    std::sort(v0.begin(), v0.end(), std::greater<>());
#endif
    print(v0);
}

struct Task {
    int prio;
    std::string name;
};
bool operator<(const Task& l, const Task& r)
{
    return l.prio < r.prio;
}
bool operator>(const Task& l, const Task& r)
{
    return l.prio > r.prio;
}
std::ostream& operator<<(std::ostream& os, const Task& t)
{
    os << "{" << t.prio << ", " << t.name << "}";
    return os;
}

void test_stable_sort()
{
    auto v = std::vector<Task>{
        { 3, "three" },
        { 7, "seven" },
        { 2, "two" },
        { 3, "three 2nd"},
        { 9, "nine"},
        { 5, "five"},
        { 3, "three 3rd"}
    };
    std::stable_sort(v.begin(), v.end());

    print(v);
}

void test_partial_sort()
{
    auto v0 = std::vector<int>{ 5, 19, 8, 4, 21, 7, 10, 6, 3, 15 };
    auto v1 = std::vector<int>(v0.size());

    std::partial_sort_copy(v0.begin(), v0.begin()+5, v1.begin(), v1.end());

    print(v1);
}

void test_nth_element()
{
    auto v0 = std::vector<int>{ 5, 19, 8, 4, 21, 7, 10, 6, 3, 15 };
    std::nth_element(v0.begin(), v0.begin()+5, v0.end());

    print(v0);

    auto sorted = std::is_sorted(v0.begin(), v0.end());
    std::cout << std::boolalpha << "sorted: " << sorted << "\n";

    auto iter = std::is_sorted_until(v0.begin()+5, v0.end());
    std::cout << "is_sorted_until(v0.begin()+5, v0.end()): " << std::distance(v0.begin(), iter) << "(" << *iter << ")\n";
}

void test_initialize()
{
    auto v0 = std::vector<int>(5);
    std::fill(v0.begin(), v0.end(), 6);

    print(v0);

    auto rd = std::random_device{};
    auto mt = std::mt19937{rd()};
    auto ud = std::uniform_int_distribution<>{1, 10};
    auto v1 = std::vector<int>(10);

    std::generate(v1.begin(), v1.end(), [&ud, &mt]{ return ud(mt); });

    print(v1);

    auto v2 = std::vector<int>(10);
    std::iota(v2.begin(), v2.end(), 10);

    print(v2);
}

void test_set()
{
    auto v0 = std::vector<int>{ 1, 1, 2, 3, 5, 8, 13 };
    auto v1 = std::vector<int>{ 2, 4, 6, 8, 10, 12, 14 };

    auto v2 = std::vector<int>{};
    std::set_union(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v2));
    std::cout << "union: ";
    print(v2);  // 1 1 2 3 4 5 6 8 10 12 13 14

    auto v3 = std::vector<int>{};
    std::set_intersection(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v3));
    std::cout << "intersection: ";
    print(v3);  // 2 8

    auto v4 = std::vector<int>{};
    std::set_difference(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v4));
    std::cout << "difference: ";
    print(v4);  // 1 1 3 5 13

    auto v5 = std::vector<int>{};
    std::merge(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v5));
    std::cout << "merge: ";
    print(v5);  // 1 1 2 2 3 4 5 6 8 8 10 12 13 14

    auto v6 = std::vector<int>{};
    std::set_symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(v6));
    std::cout << "set_symmetric_difference: ";
    print(v6);  // 1 1 3 4 5 6 10 12 13 14

    auto v7 = std::vector<int>{ 1, 3, 13 };
    std::cout << std::boolalpha << "includes: " << std::includes(v0.begin(), v0.end(), v7.begin(), v7.end()) << "\n";
}

} //namespace =================================================================

void test_ch_05_algorithm()
{
#if (0) // done
    test_find();
    test_sort();
    test_stable_sort();
    test_partial_sort();
    test_nth_element();
    test_initialize();
#endif

    test_set();
}
