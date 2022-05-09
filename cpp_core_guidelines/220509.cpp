#include <iostream>
#include <span>
#include <iterator>
#include <cstdio>

namespace { //=================================================================

// ES.42 Keep use of pointers simple and straightforward

void print(std::span<int> a)
{
    std::copy(std::begin(a), std::end(a), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void f(std::span<int> a)
{
    if (a.size() < 2)
        return;

    auto q = a.subspan(1, 3);
    std::cout << "a.subspan(1, 3) : ";
    print(q);

    a[a.size()] = 63;
    std::cout << "a[size]" << a[a.size()] << std::endl;
}

// ES.43 Avoid expressions with undefined order of evaluation

// ES.44 Don't depend on order of evaluation of function arguments

// ES.45 Avoid "magic constants"; use symbolic constants

// ES.46 Avoid loosy (narrowing, truncating) arithmetic conversions

// ES.47 Use nullptr rather than 0 or NULL

void g(int)
{
    std::cout << "g(int)" << std::endl;
}
void g(const char*)
{
    std::cout << "g(const char*)" << std::endl;
}

// ES.48 Avoid casts

} // namespace ================================================================

void test_es_42()
{
    int a[] { 1, 2, 3, 4, 5 };

    std::span<int> s0(a);
    std::cout << "a: ";
    print(s0);

    f(s0);
}

void test_es_47()
{
    std::cout << "g(0): ";
    g(0);

    std::cout << "g(nullptr): ";
    g(nullptr);
}
