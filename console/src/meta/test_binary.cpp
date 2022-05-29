#include <iostream>

namespace { //=================================================================

// loop iteration is not possible at compile time,
// and if iteration is necessary, recursion must be used
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
#if (0)
    N = N ? binary(N/10)*2 + N%10 : 0;
#else
    unsigned result = 0;
    for (unsigned bit = 0x1; N; N/=10, bit <<= 1) {
        if (N % 10)
            result += bit;
    }
    return result;
#endif
}
} //runtime

/*
 * expr = (term[expr.val = _1] >> '+' >> expr[expr.val += _1]) |
 *        (term[expr.val = _1] >> '-' >> expr[expr.val -= _1]) |
 *        term[expr.val] = _1;
 * term = (factor[term.val] = _1] >> '*' >> term[term.val *= _1]) |
 *        (factor[term.val] = _1] >> '/' >> term[term.val /= _1]) |
 *        factor[term.val] = _1;
 * factor = integer[factor.val = _1] |
 *        ('(' >> expr[factor.val = _1] >> ')');
 */

} //namespace =================================================================

void meta_test_binary()
{
    std::cout << "binary<101010>::value: " << compiletime::binary<101010>::value << std::endl;
    std::cout << "binary(101010) : " << runtime::binary(101010) << std::endl;
}
