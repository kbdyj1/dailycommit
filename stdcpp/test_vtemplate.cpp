#include <iostream>
#include <string>

template <typename T>
constexpr T pi = T(3.141592653589793238462673383);

template <>
constexpr const char* pi<const char*> = "pi";

void test_vtemplate()
{
    std::cout << pi<int> << std::endl;
    std::cout << pi<float> << std::endl;
    std::cout << pi<const char*> << std::endl;
}
