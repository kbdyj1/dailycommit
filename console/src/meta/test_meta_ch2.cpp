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

template <typename Iter>
void iterSwap(Iter i0, Iter i1)
{
    std::swap(*i0, *i1);
}

void f(int* i0, int* i1)
{
    iterSwap(i0, i1);
}

struct BitProxy
{
    BitProxy& operator=(bool x)
    {
        if (x)
            bytes[pos/8] |= (1u << (pos % 8));
        else
            bytes[pos/8] &= ~(1u << (pos % 8));

        return *this;
    }
    operator bool() const
    {
        return bytes[pos/8] & (1u << (pos % 8));
    }
    unsigned char* bytes;
    size_t pos;
};

struct BitIterator
{
    typedef bool value_type;
    typedef BitProxy reference;
    BitProxy operator*() const;
};

} // namespace ================================================================

void test_meta_ch2()
{
    int i0 = 6;
    int i1 = 3;
    f(&i0, &i1);

    std::cout << "i0: " << i0 << ", i1: " << i1 << std::endl;
}
