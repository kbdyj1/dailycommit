//=============================================================================
//  A tour of C++
//  Ch.03  Module
//
//  22/09/13
//=============================================================================

#include <iostream>
#include <string>
#include <map>
#include <complex>
#include <iterator>

#if 201703L < __cplusplus // c++20
#define USE_MODULE
#endif

#ifdef USE_MODULE
import Vector;

double sqrtSum(const Vector& v)
{
    auto sum = 0.0;
    for (auto i=0; i<v.size(); i++) {
        sum += v[i];
    }
    return sum;
}
#endif

namespace { //=================================================================

void test_static_assert()
{
    static_assert(4 <= sizeof(int));
}

struct Entry {
    int id;
    std::string name;
};

Entry create(int id) {
    return Entry{id, "unnamed"};
}

void test_structured_binding()
{
    auto [i, n] = create(0);
    std::cout << "Entry(id: " << i << ", name: " << n << ")\n";

    auto m = std::map<int, std::string>{
        { 1, "one" },
        { 2, "two" }
    };
    for (const auto& [key, value] : m) {
        std::cout << "key: " << key << ", value: " << value << "\n";
    }
}

void test_initializerlist(std::initializer_list<int> list)
{
    std::copy(std::begin(list), std::end(list), std::ostream_iterator<int>(std::cout, ","));
}

} //namespace =================================================================

void test_ch_03()
{
#if (0) // done
    test_static_assert();
    test_structured_binding();
#endif

    test_initializerlist({1, 2, 3, 4, 5});
}
