#include <iostream>
#include <iterator>

namespace { // namespace ======================================================

template <typename Iter1, typename Iter2>
void iterSwap(Iter1 i1, Iter2 i2)
{
    typename std::iterator_traits<Iter1>::value_type tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}

void f(int* i0, int* i1)
{
    iterSwap(i0, i1);
}

} // namespace ================================================================

void test_meta_ch2()
{
    int i0 = 6;
    int i1 = 3;
    f(&i0, &i1);

    std::cout << "i0: " << i0 << ", i1: " << i1 << std::endl;
}
