#include <iostream>
#include <vector>

using namespace std;

void test_ch1();
void test_ch3();
void test_ch3_3_3way_comparision();
void test_ch3_4_initialize();
void test_ch3_5_const();
void test_ch3_6();
void test_ch3_7();
void test_ch3_8();
void test_ch3_9();
void test_ch4_2();
void test_ch4_3();
void test_ch4_4();
void test_ch4_7();

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
    //test_ch3();
#endif
    //test_ch3_3_3way_comparision();
    //test_ch3_4_initialize();
    //test_ch3_5_const();
    //test_ch3_6();
    //test_ch3_7();
    //test_ch3_8();
    //test_ch3_9();
    //test_ch4_2();
    //test_ch4_3();
    //test_ch4_4();
    test_ch4_7();

    return 0;
}
