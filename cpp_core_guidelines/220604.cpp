#include <iostream>
#include <array>
#include <memory.h>
#include <ranges>
#include <vector>

namespace { //=================================================================

// SL.1 Use libraries wherever possible

// SL.2 Prefer the standard library to other libraries

// SL.3 Do not add non-standard entities to namespace std

// SL.4 Use the standard library in a type-safe manner

// SL.con.1 Prefer using STL array or vector instead of a C array

void sl_con_1()
{
    constexpr int SIZE = 16;

    int v0[16];
    std::array<int, SIZE> v1;
}

// SL.con.2 Prefer using STL vector by default unless you have a reason to use a different container
//  vector and array (con)
//      1. the fastest general-purpose access (random access)
//      2. the fastest default access pattern
//      3. the lowest space overhead

// SL.con.3 Avoid bounds errors

void f(std::vector<int>& v, std::array<int, 10> a, int i)
{
    try {
#if (0)
        v[0] = a[0];    // SIGSEGV
#else
        v.at(0) = a[0]; // SIGABORT
                        // vector::_M_range_check: __n (which is 0) >= this->size() (which is 0)
#endif
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void sl_con_3()
{
    auto a = std::array<int, 10>();

    memset(a.data(), 0, 10);    // bad. wrong size
    a.fill(0);
    std::fill(std::begin(a), std::end(a), 0);

    auto v = std::vector<int>();

    f(v, a, 0);
}

// SL.con.4 Don't use memset or memcpy for arguments that are not trivially-copyable

struct Base {
    virtual void update() = 0;
};
struct Derived : public Base {
    void update() override {
        std::cout << "Derived::update()" << std::endl;
    }
};

void sl_con_4()
{
    auto d = Derived{};
    memset(&d, 0, sizeof(Derived));    
    d.update();

    Base& base = d;
    base.update();
}

} //===========================================================================

void test_sl_con()
{
    //sl_con_3();
    sl_con_4();
}
