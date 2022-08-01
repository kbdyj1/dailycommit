#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <bitset>
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

namespace item17 {

void test()
{
    std::vector<int> v;
    for (auto i=0; i<1024; i++) {
        v.push_back(i);
    }
    v.erase(std::remove_if(v.begin(), v.end(), [](int i){
        return i % 10 != 0;
    }), v.end());
    std::cout << "#1 size: " << v.size() << ", capacity: " << v.capacity() << "\n";

#if (0)
    std::vector<int>(v).swap(v);
#else
    v.shrink_to_fit(); // c++11
#endif
    std::cout << "#2 size: " << v.size() << ", capacity: " << v.capacity() << "\n";
}

void test_string()
{
    auto s = std::string{"Debugging of /Users/administrator/project/dailycommit/build-stl-Desktop_arm_darwin_generic_mach_o_32bit-Debug/stl has finished."};
    std::cout << "#1 size: " << s.size() << ", capacity: " << s.capacity() << "\n";
    s.erase(std::remove_if(s.begin(), s.end(), [](char c){
        return c == ' ';
    }), s.end());
#if (1)
    //std::string(s).swap(s);
#else
    s.shrink_to_fit();
#endif
    std::cout << "s: " << s << "\n";
    std::cout << "#2 size: " << s.size() << ", capacity: " << s.capacity() << "\n";
}

} // item17 ---------------------------------------------------------

namespace item18 {

void test()
{
#if (0)
    std::vector<bool> v{ true, true, false, true };
#else
    std::deque<bool> v{ true, true, false, true };
#endif
    for (auto iter=v.begin(); iter!=v.end(); iter++) {
        std::cout << *iter << "\n";
    }
    std::bitset<4> o(0xA);
    std::cout << "std::bitset<4>(0xA): " << o.to_string() << "\n";
}

} // item18 ---------------------------------------------------------

} // namespace ================================================================

void test_ch_2()
{
    //item17::test_string();
    item18::test();
}
