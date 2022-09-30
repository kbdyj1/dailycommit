#include <iostream>

namespace { //=================================================================

template <typename, int>
class A {};

void test_unnamed_parameter()
{
    auto a = A<int, 8>{};
}

} //namespace =================================================================

void test_ch_12_template_parameter()
{
    test_unnamed_parameter();
}
