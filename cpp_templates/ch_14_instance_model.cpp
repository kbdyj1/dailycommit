#include <iostream>
#include "ch_14_t.hpp"

namespace { //=================================================================

namespace _1 {

namespace N {

template <typename>
void g()
{}

enum E { e };

} //N ---------------------------------------------------------------

template <typename>
void f()
{}

template <typename T>
void h(T t)
{
    f<int>();
//    g<int>();
}

void test()
{
    h(N::e);
}

} //_1 --------------------------------------------------------------

#if (0)
namespace _2 {

class My {
public:
    My(int);
};

My operator-(const My&);
bool operator<(const My&, const My&);

using Type = My;

template <typename T>
void func(T t)
{
    if (0 < t)
        g(-t);
}

void g(Type)
{
    func<Type>(42);
}

} //_2 --------------------------------------------------------------
#endif

namespace _3 {

void g0(int)
{}

template <typename T>
void f0(T x)
{
    g0(x);
}

void test0()
{
    f0(7);
}

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename T>
class S {
public:
    T m;
};

unsigned long h()
{
    return (unsigned long)sizeof(S<int>);
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename T>
class S {
public:
    using I = int;
};

template <typename T>
void f()
{
    auto v0 = S<char>::I{41};
    auto v1 = typename S<T>::I{42};
}

void test()
{
    f<double>();
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_14_instance_model()
{
#if (0) //done
    _1::test();
    _5::test();
#endif

    externalFunc<int>();
}
