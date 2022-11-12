//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Appendix
//
//  22/11/12
//=============================================================================

#include <iostream>
#include <typeinfo>

#define USE_DYNAMIC

class Decider {
public:
#if defined(USE_DYNAMIC)
    virtual ~Decider()
    {}
#endif
};

extern Decider decider;

namespace { //=================================================================

namespace _1 {

int test()
{
    auto name = typeid (decider).name();
    return (int) sizeof(decider);
}

} //_1 --------------------------------------------------------------

namespace _2 {

class A {
public:
    A()
    {}
    A(const A&) = delete;
    A(A&&) = delete;
};

A makeA() {
    return A();
}

void test()
{
    auto a = makeA();
}

} //_2 --------------------------------------------------------------

namespace _3 {

int& lvalue();
int&& xvalue();
int prvalue();

void test()
{
    std::cout << std::boolalpha;

    std::cout << std::is_same_v<decltype(lvalue()), int&> << '\n';
    std::cout << std::is_same_v<decltype(xvalue()), int&&> << '\n';
    std::cout << std::is_same_v<decltype(prvalue()), int> << '\n';
}

} //_3 --------------------------------------------------------------

namespace _4 {

int f1(int) {}
int f1(double) {}

int f2(int) {}
int f2(char) {}


class A {
public:
    A(int)
    {}
};
int f3(A) {}
int f3(...) {}

void test()
{
    // perfect match
    f1(4);

    // by promotion
    f2(true);

    // conversion operator
    f3(7);
}

} //_4 --------------------------------------------------------------

namespace _5 {

template <typename T>
class A {
public:
    A()
    {}
    A(const T*)
    {}
};

template <typename T>
A<T> truncate(const A<T>&, int)
{}

void test()
{
    A<char> s0, s1;

    s0 = truncate<char>("Hello, Qt6", 0);
#if (0)
    s1 = truncate("Ooops!!!", 0);
#endif
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
void strange(T&&, T&&)
{}

template <typename T>
void bbb(T&&, double&&)
{}

void test()
{
    strange(1.0, 3.0);
    double val = 1.0;
    strange(val, val);
#if (0) //error
    strange(1.0, val);
    bbb(val, val);
#endif
}

} //_6 --------------------------------------------------------------

namespace _7 {

class BadString {
public:
    BadString(const char*);

    char& operator[](std::size_t);
    const char& operator[](std::size_t) const;

    operator char*();
    operator const char*();
};

class A {
public:
    void f0()
    {}
    void f1()&&
    {}
    void f2()&
    {}
};

void test()
{
    A().f0();
    A().f1();
#if (0)
    A().f2();
#endif
}

} //_7 --------------------------------------------------------------

} //namespace =================================================================

void test_appendix()
{
#if (0) //done
    _1::test();
    _3::test();
#endif

    _6::test();
}
