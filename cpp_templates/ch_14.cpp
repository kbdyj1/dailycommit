//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.14 Template instantiation
//
//  22/10/06
//=============================================================================

#include <iostream>


namespace { //=================================================================

template <typename T> class A;

A<int>* p = nullptr;

template <typename T>
class A {
public:
    void f();
};

void g(A<int>& c)
{
    c.f();
}

template <typename T>
void A<T>::f()
{
    std::cout << "A<T>::f()\n";
}

void test()
{
    auto d = A<int>{};

    g(d);

    p->f();
}

//-------------------------------------------------------------------

template <typename T>
class B {
public:
    B(int);
};

void func(B<double>);
void func(int)
{}

void test_func()
{
    func(42);
}

} //namespace =================================================================

void test_ch_14_lazy();
void test_ch_14_instance_model();

void test_ch_14()
{
#if (0) //done
    test();
    test_func();
    test_ch_14_lazy();
#endif

    test_ch_14_instance_model();
}
