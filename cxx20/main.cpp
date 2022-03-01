#include <iostream>
#include <vector>

using namespace std;

void test_ch1();
void test_ch3();
void test_ch3_3_3way_comparision();
void test_ch3_4_initialize();
void test_ch3_5_const();

#ifdef USE_CXX20_FEATURE_MODULE
import math;

void test_ch3_2_module()
{
    auto result = add(1, 2);

    auto vec = std::vector<int>{1, 2, 3, 4, 5};
    auto ret = getProduct(vec);
}
#endif

int main()
{
    //test_ch1();
#ifndef OS_MAC
    test_ch3();
#endif
    //test_ch3_3_3way_comparision();
    //test_ch3_4_initialize();
    test_ch3_5_const();

    return 0;
}
