#include <iostream>

namespace { //=================================================================

// ES.100 Don't mix signed and unsigned arithmetic

// ES.101 Use unsigned types for bit manipulation

// ES.102 Use signed types for arithmetic

// ES.103 Don't overflow

// ES.104 Don't underflow

// ES.105 Don't divide by integer zero

// ES.106 Don't try to avoid negative values by using unsigned

// ES.107 Don't use unsigned for subscripts, prefer gsl::index

template <typename T0, typename T1>
T0 subtract(T0 x, T1 y)
{
    return x - y;
}

} // namespace ================================================================

void test_es_100()
{
    int x = -3;
    unsigned int y = 7;

    std::cout << "x: " << x << ", y: " << y << std::endl;
    std::cout << "x - y: " << x - y << std::endl;
    std::cout << "x + y: " << x + y << std::endl;
    std::cout << "x * y: " << x * y << std::endl;
}

void test_es_101()
{
    int s = 5;
    unsigned int us = 5;
    std::cout << subtract(s, 7) << std::endl;
    std::cout << subtract(us, 7u) << std::endl;
}
