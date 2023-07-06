#include <string.h>
#include <iostream>
#include <QString>
#include <QDebug>
#include <exception>

namespace { //=================================================================

namespace _1 {

void test()
{
    char* s = strdup("Hello, Qt6");
    auto str = QString::fromStdString(s);
    qDebug() << str;

    free(s);
}

} //_1 --------------------------------------------------------------

namespace _2 {

struct B;
struct A {
    A() { std::cout << "A()\n"; }
    ~A() { std::cout << "~A()\n"; }
    std::shared_ptr<B> b;
};

//#define MAKE_LEAK

struct B {
    B() { std::cout << "B()\n"; }
    ~B() { std::cout << "~B()\n"; }
#if defined(MAKE_LEAK)
    std::shared_ptr<A> a;
#else
    std::weak_ptr<A> a;
#endif
};

void test()
{
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();
    a->b = b;
    b->a = a;
}

} //_2 --------------------------------------------------------------

namespace _3 {

class E {
public:
    E() {
        std::cout << "E()\n";
        throw std::exception();
    }
    ~E() { std::cout << "~E()\n"; }
};
class A {
public:
    A() { std::cout << "A()\n"; }
    ~A() { std::cout << "~A()\n"; }
};

class S {
public:
    S() : a(new A)
        , e(new E)
    {
        std::cout << "S()\n";
    }
    ~S() {
        std::cout << "~S()\n";

        delete a;
        delete e;
    }
    A* a;
    E* e;
};

void test()
{
    try {
        S s;
    } catch (std::exception& e) {
        std::cerr << "exception.\n";
    }
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ownership()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
