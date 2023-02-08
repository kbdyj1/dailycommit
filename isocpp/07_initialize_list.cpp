#include <iostream>
#include <initializer_list>
#include <utility>
#include <vector>

namespace { //=================================================================

namespace _1 {

void foo(std::initializer_list<int> l)
{
    if (!std::empty(l)) {
        for (const auto& i : l) {
            std::cout << i << ' ';
        }
        std::cout << "(" << "elements: " << l.size() << ")\n";
    } else {
        std::cout << "empty list\n";
    }
}

void foo(const std::vector<int>& l)
{
    std::cout << "vector<int>\n";
}

void test()
{
    foo({});
    foo({1, 2, 3, 4, 5});

#if (0)
    foo({1, 2, 3.5}); //narrow error
#endif
    foo({1, 'x', '0', 10});

    auto v = std::vector<int>{ 10, 11, 12 };
    foo(v);
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_initialize_list()
{
    _1::test();
}
