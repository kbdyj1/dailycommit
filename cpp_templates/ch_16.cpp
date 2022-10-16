//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.16 Specialization & overloading
//
//  22/10/16
//=============================================================================

#include <iostream>

namespace { //=================================================================

namespace _1 { //transparency customizing

template <typename T, size_t N=16>
class Array {
    T* data;
public:
    Array() : data(new T[N])
    {}

    Array(const Array<T>& other)
    {}
    Array& operator=(const Array& other)
    {
        return *this;
    }
    ~Array()
    {
        delete[] data;
    }
    void exchangeWith(Array* other)
    {
        T* tmp = data;
        data = other->data;
        other->data = tmp;
    }
    T& operator[](std::size_t i) {
        return data[i];
    }
    size_t size() const {
        return N;
    }
};

template <typename T>
inline void exchange(T* a, T* b)
{
    T tmp(*a);
    *a = *b;
    *b = tmp;
}

template <typename T>
inline void exchange(Array<T>* a, Array<T>* b)
{
#if (0)
    a->exchangeWith(b);
#else
    T* pa = &(*a)[0];
    T* pb = &(*b)[0];
    for (auto items = a->size(); --items; ) {
        exchange(pa++, pb++);
    }
#endif
}

void test()
{
    auto a0 = Array<int>{};
    auto a1 = Array<int>{};
    auto i0 = 10;
    auto i1 = 20;

    auto*pa = &a0[0];
    a0[0] = 0;
    a1[0] = 1;

    exchange(&a0, &a1);
    exchange(&i0, &i1);

    std::cout << "&a0[0]: " << *pa << "\n";
    std::cout << "int: " << i0 << ", " << i1 << "\n";
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_16_overloading();

void test_ch_16()
{
#if (0) //done
    _1::test();
#endif
    test_ch_16_overloading();
}
