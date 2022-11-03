//=============================================================================
//  C++ templates. the complete guide. 2nd
//
//  22/09/19 ~
//=============================================================================

#include <iostream>

template <typename T>
void external()
{
    std::cout << "template<typename T>\nexternal()\n";
}

template<>
void external<int>()
{
    std::cout << "external<int>()\n";
}

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
void test_ch_13();
void test_ch_14();
void test_ch_15();
void test_ch_16();
void test_ch_18();
void test_ch_19();
void test_ch_20();
void test_ch_21();
void test_ch_22();
void test_ch_23();
void test_ch_24();

int main()
{
#if (0)
    auto odr = ODR{};
#endif
    test_ch_24();

    return 0;
}
