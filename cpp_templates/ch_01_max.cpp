#include <iostream>

namespace { //=================================================================

template <typename T>
T max(T a, T b)
{
    std::cout << "max<>(" << a << ", " << b << ")\n";
    return b < a ? a : b;
}

template <typename T>
T max(T a, T b, T c)
{
    return max(max(a, b), c);
}

int max(int a, int b)
{
    std::cout << "int max(" << a << ", " << b << ")\n";
    return b < a ? a : b;
}

} //namespace =================================================================

void test_ch_01_max()
{
    max(47, 11, 33);
}
