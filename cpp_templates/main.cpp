//=============================================================================
//  C++ templates. the complete guide. 2nd
//
//  22/09/19 ~
//=============================================================================

#include <iostream>

namespace { //=================================================================

} //namespace =================================================================

class ODR
{
public:
    ODR() { std::cout << "main ODR{}\n"; }
};

void test_ch_01();
void test_ch_02();
void test_ch_03();
void test_ch_04();
void test_ch_05();
void test_ch_06();
void test_ch_07();
void test_ch_08();
void test_ch_09();
void test_ch_10();
void test_ch_11();
void test_ch_12();

int main()
{
#if (0)
    auto odr = ODR{};
#endif
    test_ch_12();

    return 0;
}
