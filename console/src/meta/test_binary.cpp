#include <iostream>

namespace { //=================================================================

namespace compiletime
{
template <unsigned long N>
struct binary
{
    static unsigned const value = binary<N/10>::value * 2 + N%10;
};

template <>
struct binary<0>
{
    static unsigned const value = 0;
};
} //compiletime

namespace runtime {
unsigned binary(unsigned long N)
{
    N = N ? binary(N/10)*2 + N%10 : 0;
}
} //runtime

} //namespace =================================================================

void meta_test_binary()
{
    std::cout << "binary<101010>::value: " << compiletime::binary<101010>::value << std::endl;
    std::cout << "binary(101010) : " << runtime::binary(101010) << std::endl;
}
