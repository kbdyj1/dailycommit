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

} // item14 ---------------------------------------------------------

namespace item15 {

struct Bar {};

// www.cantrip.org/emptyopt.html

// a class with an empty sequence of members and base class objects is an empty class.
// Complete objects and member subobjects of an empty class type shall have nonzero size.
struct Foo {
    Bar b;
    int* p;
};

// a base class subobject of an empty class type may have zero size
struct Baz : Bar {
    int* p;
};

void test()
{
    auto s = std::string{};

    std::cout << "sizeof(std::string): " << sizeof(std::string) << "\n";

    std::cout << "sizeof(Foo): " << sizeof(Foo) << "\n";
    std::cout << "sizeof(Baz): " << sizeof(Baz) << "\n";
}

} // item15 ---------------------------------------------------------

namespace item16 {

void legacy_api(int* ptr, size_t len)
{
}
void legacy_api(const char* ptr)
{
}

void test()
{
    auto v = std::vector<int>{};
    if (!v.empty()) {
        legacy_api(&v[0], v.size());
    }

    auto s = std::string{"Hello, Qt 6"};
    legacy_api(s.c_str());
}

} // item16 ---------------------------------------------------------

} // namespace ================================================================

void test_ch_2()
{
    item15::test();
}
