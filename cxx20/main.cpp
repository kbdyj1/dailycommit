#include <iostream>
#include <vector>

using namespace std;

void test_ch1();
void test_ch3();

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
    test_ch3();

    return 0;
}
