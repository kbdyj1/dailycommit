//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.08 Compile time programming
//
//  22/09/27
//=============================================================================
#include <iostream>

namespace { //=================================================================

} //namespace =================================================================

void test_ch_08_meta_programming();
void test_ch_08_sfinae();
void test_ch_08_constexpr();

void test_ch_08()
{
#if (0)
    test_ch_08_sfinae();
    test_ch_08_meta_programming();
#endif

    test_ch_08_constexpr();
}
