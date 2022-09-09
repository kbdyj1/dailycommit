#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>
#include <sstream>
#include <iterator>

#include "util.h"

namespace { //=================================================================

class A {
    int a;
    double b;
    std::string str;

public:
    A(const int a = 0, const double b = 0.0, const std::string& s = "") : a(a), b(b), str(s)
    {}
    ~A() {
        std::cout << "~A(): ";
        print();
    }
    A& operator=(const A& other) {
        a = other.a;
        b = other.b;
        str = other.str;
    }

    void print() const {
        std::cout << "A(a:" << a << ", b:" << b << ", str:" << str << ")\n";
    }
};
struct ADestructor {
    void operator()(A* a) const {
        std::cout << "delete a. ";
        delete a;
    }
};

void test_unique_ptr()
{
#if (0)
    auto null = std::unique_ptr<int>{};
    auto pi = std::unique_ptr<int>{new int{62}};
    auto parri = std::unique_ptr<int[]>{new int[3]{1,2,3}};
    auto pa = std::unique_ptr<A>{new A{1, 2.0, "Qt 6.2"}};
#else
    //c++14
    auto pi = std::make_unique<int>(62);
    auto parri = std::make_unique<int[]>(3);    // 3 elements
    auto pa0 = std::make_unique<A>(1, 2.0, "Qt6.2");
#endif

    pa0->print();

    auto pa1 = std::unique_ptr<A, ADestructor>(new A{2, 4.0, "Qt5.15"}, ADestructor());
    pa1->print();

    auto pa2 = std::move(pa1);
    std::cout << "pa1 = " << typeid(pa1).name() << ", pa2 = " << typeid(pa2).name() << std::endl;

    assert(pa1.get() == nullptr);
    assert(pa2.get() != nullptr);

    auto* pa3 = pa2.release();
    std::cout << "pa3: ";
    pa3->print();

    delete pa3;
}

void test_unique_ptr_release()
{
    auto pa0 = std::unique_ptr<A, ADestructor>(new A(1, 2.0, "Hello"), ADestructor());
    *pa0 = A{2, 4.0, "No"};
    if (pa0) {
        pa0->print();
    }

    auto pi = std::make_unique<int>(1);
    *pi = 2;
    if (pi) {
        std::cout << "pi: " << *pi << std::endl;
    }
}

void test_unique_ptr_array()
{
    auto v = std::vector<std::unique_ptr<A>>{};
    for (auto i=0; i<3; i++) {
        auto s = std::stringstream{};
        s << "Qt6." << i;
        v.push_back(std::make_unique<A>(i, i*2.0, s.str()));
    }

    auto pa = std::make_unique<A>(4, 8.0, "Qt6.4");
    v.push_back(std::move(pa));
}

void func(std::unique_ptr<A> pa, const int ci)
{}

int throwFunc()
{
    throw std::runtime_error("not implemented");
}

void test_unique_ptr_leak()
{
    try {
#if (1)
    func(std::unique_ptr<A>(new A(1, 2.0, "Qt6.2")), throwFunc());  // possible leak
#else
    func(std::make_unique<A>(1, 2.0, "Qt6.2"), throwFunc());
#endif
    } catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void test_shared_ptr()
{
    auto pa0 = std::shared_ptr<int[]>(new int[3]{1,2,3}, std::default_delete<int[]>());
    auto pa1 = std::shared_ptr<int[]>(new int[3]{4,5,6}, [](auto p){
        std::copy(p, p+3, std::ostream_iterator<int>(std::cout, " "));
        std::cout << "\n";
        std::cout << typeid(p).name() << std::endl;
        delete [] p;
    });

#if 201703L <= __cplusplus
    for (auto i=0; i<3; i++)
        std::cout << pa1[i] << (i < 2 ? "," : " ");

    std::cout << "\n";
#endif
}

void test_weak_ptr()
{
    auto sp0 = std::make_shared<int>(62);
    PRINT_FUNC(sp0.use_count());

    auto wp0 = std::weak_ptr(sp0);
    std::cout << "weak_ptr(sp0): ";
    PRINT_FUNC(sp0.use_count());

    auto sp1 = wp0.lock();
    std::cout << "wp0.lock(): ";
    PRINT_FUNC(sp0.use_count());

    sp1.reset();
    std::cout << "sp1.reset(): ";
    PRINT_FUNC(sp0.use_count());
}

struct B;
struct C : public std::enable_shared_from_this<C>
{
    ~C() { std::cout << "~C()\n"; }

    void takeB(std::shared_ptr<B> b);

private:
    std::shared_ptr<B> b;
};

struct B
{
    ~B()
    {
        std::cout << "~B()\n";
        PRINT_FUNC(c.expired());
    }

    void takeC(std::weak_ptr<C> c) {
        this->c = c;
    }
private:
    std::weak_ptr<C> c;
};

void C::takeB(std::shared_ptr<B> b)
{
    this->b = b;
    this->b->takeC(shared_from_this());
}

void test_shared_from_this()
{
    auto b = std::make_shared<B>();
    auto c = std::make_shared<C>();
    c->takeB(b);
}

struct Base {
    virtual void print() {
        std::cout << "Base::print()\n";
    }
};
struct Derived : public Base {
    void print() override {
        std::cout << "Derived::print()\n";
    }
};

void test_static_pointer_cast()
{
    auto pd = std::make_shared<Derived>();
    auto pb = std::shared_ptr<Base>(pd);

#if 201703L <= __cplusplus
    std::static_pointer_cast<Derived>(pb)->print();
    pb->print();
#endif
}

} //namespace =================================================================

void test_ch_09_resource()
{
    std::cout << std::boolalpha;

#if (0)
    test_unique_ptr();
    test_unique_ptr_release();
    test_unique_ptr_array();
    test_unique_ptr_leak();
    test_shared_ptr();
    test_weak_ptr();
    test_static_pointer_cast();
#endif
    test_shared_from_this();


    std::cout << std::endl;
}
