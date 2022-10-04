//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.13 Template name
//
//  22/10/04
//=============================================================================

#include <iostream>

namespace { //=================================================================

namespace big {

class Number {
public:
    int value;
};
bool operator<(const Number& a, const Number& b) {
    return a.value < b.value;
}

} //big -------------------------------------------------------------

template <typename T>
T max(T a, T b)
{
    return a < b ? b : a;
}

void g(const big::Number& a, const big::Number& b)
{
    auto c = max(a, b);

    std::cout << c.value << "\n";
}

void test_big_number()
{
    auto bn0 = big::Number{};
    bn0.value = 4;

    auto bn1 = big::Number{};
    bn1.value = 8;

    g(bn0, bn1);
}

template <bool B>
class Invert {
public:
    static const bool result = !B;
};

void test_invert()
{
    auto test = Invert<(1>0)>::result;
    std::cout << "Invert<(1>0)>::result: " << test << "\n";
}

} //namespace =================================================================

void test_ch_13_adl();
void test_ch_13_hack();
void test_ch_13_dependency();

void test_ch_13()
{
    std::cout << std::boolalpha;

#if (0) //done
    test_big_number();
    test_ch_13_adl();
    test_invert();
    test_ch_13_hack();
#endif

    test_ch_13_dependency();
}
