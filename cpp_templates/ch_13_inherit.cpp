#include <iostream>

namespace { //=================================================================

namespace _1 {

template <typename X>
class B {
public:
    int baseField;
    using T = int;
};

class D0 : public B<B<void>> {
public:
    void f() { baseField = 3; }
};

template <typename T>
class D1 : public B<double> {
public:
    void f() { baseField = 7; }

    T strange;  // Base<double>::T = int
};

void g(D1<int*>& d, int* p)
{
#if (0)
    d.strange = p;
#endif
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
class B {
public:
    int baseField;
};

template <typename T>
class D : public B<T> {
public:
#if (1)
    using B<T>::baseField;
    void f() { baseField = 0; }
#else
    void f() {
        //this->baseField = 0;
        B<T>::baseField = 0;
    }
#endif
};

template <>
class B<bool> {
public:
    enum { baseField = 42 };
};

#if (0) //error
void g(D<bool>& d)
#else
void g(D<int>& d)
#endif
{
    d.f();
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T>
class B {
public:
    enum E {
        e1,
        e2,
        e3
    };
    virtual void zero(E e = e1);
    virtual void one(E& e);
};

template <typename T>
class D : public B<T> {
public:
    void f() {
        typename D<T>::E e;
        this->zero();
        one(e);
    }
};

} //_3 --------------------------------------------------------------

namespace _4 {

class NonDep {
public:
    using Type = int;
};

template <typename T>
class Dep {
public:
    using OtherType = T;
};

template <typename T>
class DepBase : public NonDep, public Dep<T> {
public:
    void f() {
        typename DepBase<T>::Type t;
        typename DepBase<T>::OtherType* ot;
    }
};

} //_4 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_13_inherit()
{

}
