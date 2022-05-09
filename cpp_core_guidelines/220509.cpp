#include <iostream>
#include <span>
#include <iterator>

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
}

} // namespace ================================================================

void test_es_42()
{
    int a[] { 1, 2, 3, 4, 5 };

    std::span<int> s0(a);
    std::cout << "a: ";
    print(s0);

    f(s0);
}
