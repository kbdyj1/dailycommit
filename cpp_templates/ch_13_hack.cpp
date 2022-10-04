#include <iostream>

namespace { //=================================================================

template <int I>
struct X {
    static const int c = 2;
};

template <>
struct X<0> {
    typedef int c;
};

template <typename T>
struct Y {
    static const int c = 3;
};

} //namespace =================================================================

static const int c = 4;

void test_ch_13_hack()
{
    std::cout << (Y<X<1> >::c >::c>::c) << ' ';
    std::cout << (Y<X< 1>>::c >::c>::c) << "\n";
}
