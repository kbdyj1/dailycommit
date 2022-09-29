//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.11 Generic library
//
//  22/09/28
//=============================================================================

#include <iostream>
#include <vector>

void test_ch_11_function();
void test_ch_11_util();
void test_ch_11_forward();

namespace { //=================================================================

template <typename T>
void IsReference(T) {
    std::cout << "IsReference(T): " << std::is_reference_v<T> << "\n";
}

void test_is_reference()
{
    int i;
    int&r = i;

    IsReference(i);
    IsReference(r);
    IsReference<int&>(i);
    IsReference<int&>(r);
}

template <typename T, T Z = T{}>
class RefMem {
    T zero;

public:
    RefMem() : zero(Z)
    {}
};

int null;

void test_ref_mem()
{
    auto r0 = RefMem<int>{};
    auto r1 = RefMem<int>{};

    auto r4 = RefMem<int&, null>{};
    auto r5 = RefMem<int&, null>{};

#if (0)
    auto r2 = RefMem<int&>{};
    auto r3 = RefMem<int&, 0>{};    // value of type 'int' is not implicitly convertible to 'int&'

    // for classes with non-static reference members, the default assignment operator is deleted.
    r4 = r5;
#endif
}

template <typename T, int& SZ>
class Array {
    std::vector<T> elems;

public:
    Array() : elems(SZ)
    {}
    void print() const {
        std::cout << "Array[" << elems.size() << "]\n";
        std::cout << "Array[SZ:" << SZ << "]\n";
    }
};

int SIZE = 32;

void test_array()
{
#if (0)
    auto a0 = Array<int&, SIZE>{};
    a0.print();
#endif

    auto a1 = Array<int, SIZE>{};
    a1.print();

    SIZE += 100;

    a1.print();
}

template <typename T>
class Container {
    T* elelms = nullptr;

public:
    template <typename D = T>
    typename std::conditional<std::is_move_constructible<D>::value, T&&, T&>::type
    foo();
};

} //namespace =================================================================


void test_ch_11()
{
    std::cout << std::boolalpha;

#if (0) //done
    test_ch_11_function();
    test_ch_11_util();
    test_ch_11_forward();
    test_is_reference();
#endif

    test_array();
}
