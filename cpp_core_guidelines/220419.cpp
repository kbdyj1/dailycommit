#include <iostream>

namespace { //=================================================================

// C66. Make move operations noexcept (no allocatoin required)

template <typename T>
class Vec
{
    T* p;
    int size;

public:
    Vec(Vec&& v) noexcept : p(v.p), size(v.size)
    {
        v.p = nullptr;
        v.size = 0;
    }
    Vec& operator=(Vec&& v) noexcept
    {
        p = v.p;
        size = v.size;
        v.p = nullptr;
        v.size = 0;
    }
};

// C67. A polymorphic class should suppress public copy/move
//      if the class has no data, =delete the copy/move functions.
//      otherwise, make them protected

namespace bad
{

class B {
public:
    virtual char c() { return 'B'; }
};

class D : public B {
public:
    char c() override { return 'D'; }
};

void f(B& b)
{
    auto b2 = b;

    std::cout << "bad::f(B&) : " << b2.c() << std::endl;
}

class Tracer
{
    std::string message;

public:
    Tracer(const std::string& m) : message(m)
    {
        std::cout << "entering " << message << std::endl;
    }
   ~Tracer()
    {
        std::cout << "exiting " << message << std::endl;
    }

    // if defined the destructor, we must define the copy and move operations.
    Tracer(const Tracer& o) : message(o.message)
    {
        std::cout << "Tracer(const Tracer&)" << std::endl;
    }
    Tracer& operator=(const Tracer& o)
    {
        std::cout << "operator=(const Tracer&)" << std::endl;
        if (this != &o) {
            message = o.message;
        }
        return *this;
    }
    Tracer(Tracer&& o) : message(std::move(o.message))
    {
        std::cout << "this : " << message << std::endl;
        std::cout << "other: " << o.message << std::endl;
    }
    Tracer& operator=(Tracer&& o)
    {
        if (this != &o) {
            std::cout << "Tracer::operator=(Tracer&&) #1 o.message -> " << o.message << std::endl;

            message = std::move(o.message);

            std::cout << "Tracer::operator=(Tracer&&) #2 o.message -> " << o.message << std::endl;
        }
        return *this;
    }
};

Tracer createTracer()
{
    auto ret = Tracer("Qt6.3");
    std::cout << "createTracer" << std::endl;
    return ret;
}

} // bad

namespace good
{

class B {
public:
    B() = default;
    B(const B&) = delete;
    B& operator=(const B&) = delete;
    virtual char c() { return 'B'; }
};

class D : public B {
public:
    char c() override { return 'D'; }
};

void f(B& b)
{
    //error: use of deleted function ‘{anonymous}::good::B::B(const {anonymous}::good::B&)’
    //   72 |     auto b2 = b;
    //      |               ^
#if (0)
    auto b2 = b;
#endif

    std::cout << "good::f(B&) : " << b.c() << std::endl;
}

class Tracer
{
    std::string message;

public:
    Tracer(const std::string& m) : message(m)
    {
        std::cerr << "entering " << message << std::endl;
    }
   ~Tracer()
    {
        std::cerr << "exiting " << message << std::endl;
    }

    // if defined the destructor, we must define the copy and move operations.
    Tracer(const Tracer&) = default;
    Tracer& operator=(const Tracer&) = default;
    Tracer(Tracer&&) = default;
    Tracer& operator=(Tracer&&) = default;
};

} // good

// C80. Use =default if you have to be explicit about using the default semantics

// C81. Use =delete when you want to disable default behavior (without wanting an alternative)

// C82. Don't call virtual functions in constructors and destructors

class Base
{
public:
    virtual void f() = 0;
    virtual void g() { std::cout << "Base::g()" << std::endl; }
    virtual void h() { std::cout << "Base::h()" << std::endl; }
    virtual ~Base()
    {}
};

class Derived : public Base
{
public:
    void f() override { std::cout << "Derived::f()" << std::endl; }
    void g() override { std::cout << "Derived::g()" << std::endl; }
    void h() final { std::cout << "Derived::h()" << std::endl; }

    Derived()
    {
        Derived::f();
        g();
        h();
    }
};

} // namespace ================================================================

void test_c_67()
{
    std::cout << "bad function" << std::endl;
    bad::D d;
    bad::f(d);

    std::cout << "good function" << std::endl;
    good::D d2;
    good::f(d2);
}

void test_c_80()
{
    bad::Tracer t1(std::move(bad::createTracer()));
}

void test_c_82()
{
    Base* p = new Derived;
    delete p;
}
