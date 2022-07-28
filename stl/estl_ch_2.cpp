#include <iostream>
#include <vector>
#include <string>
#include "a.h"

namespace { //=================================================================

namespace item14 {

void test()
{
    // Prefre vectors and strings over dynamically allocated arrays.

    A a;

    std::vector<A> v;
    std::cout << "v.capacity(): " << v.capacity() << "\n";

    v.push_back(A(1));
    v.push_back(A(2));
    std::cout << "v.capacity(): " << v.capacity() << "\n";

    std::cout << "start v.reserve(4)\n";
    v.reserve(4);
    std::cout << "end v.reserve(4)\n";

    std::cout << "start v.resize(1)\n";
    v.resize(1);
    std::cout << "end v.resize(1)\n";
}

} // item13 ---------------------------------------------------------

} // namespace ================================================================

void test_ch_2()
{
    item14::test();
}
