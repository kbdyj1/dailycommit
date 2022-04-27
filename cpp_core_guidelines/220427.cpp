#include <iostream>
#include <memory>

namespace { //=================================================================

// C139. Use final on classes sparingly

// C140. Do not provide different default arguments for a virtual function and an overrider

namespace bad
{

class Base {
public:
    virtual int multiply(int value, int factor=2) = 0;
    virtual ~Base() = default;
};

class Derived : public Base
{
public:
    int multiply(int value, int factor=10) override
    {
        return value * factor;
    }
};
} // bad

// C145. Access polymorphic objects through pointers and references

// C146. Use dynamic_cast where class hierarchy navigation is unavoidable

// CRTP (Curiously recurring template pattern)

class Shape
{
public:
    virtual ~Shape() = default;
    virtual Shape* clone() const = 0;
};

namespace CRTP
{

template <typename T>
struct Base
{
    void interface()
    {
        static_cast<T*>(this)->implementation();
    }
    static void staticCall()
    {
        T::staticCall();
    }
};

template <typename T>
struct Counter
{
    static int objectCreated;
    static int objectAlive;

    Counter()
    {
        ++objectCreated;
        ++objectAlive;
    }
protected:
    ~Counter()
    {
        --objectAlive;
    }
};
template <typename T> int Counter<T>::objectAlive = 0;
template <typename T> int Counter<T>::objectCreated = 0;

template <typename Derived>
class ClonableShape : public Shape
{
public:
    Shape* clone() const
    {
        return new Derived(static_cast<Derived const&>(*this));
    }
};

} // CRTP

struct Derived : CRTP::Base<Derived>
{
    void implementation()
    {
        std::cout << "Derived::implementation()" << std::endl;
    }
    static void staticCall()
    {
        std::cout << "Derived::staticCall()" << std::endl;
    }
};

class X : CRTP::Counter<X>
{
};
class Y : CRTP::Counter<Y>
{
};

class Square : public CRTP::ClonableShape<Square>
{
    int x0;
    int y0;
    int x1;
    int y1;
public:
    Square(int x0, int y0, int x1, int y1) : x0(x0), y0(y0), x1(x1), y1(y1)
    {}
    Square(const Square& other) = default;
    Square& operator=(const Square& other) = default;

    void print()
    {
        std::cout << "Square(" << x0 << ", " << y0 << ", " << x1 << ", " << y1 << ")" << std::endl;
    }
};

// C147. Use dynamic_cast to a reference type when failure to find the required class is considered an error

class C
{
    std::string str;
public:
    C(const std::string& s) : str(s)
    {}
    virtual ~C() = default;

    C(const C&) = default;
    C& operator=(const C&) = default;

    std::string name() const
    {
        return str;
    }
};
class D : public C
{
public:
    D() : C("D")
    {}
};
class E : public C
{
public:
    E() : C("E")
    {}
};

std::string className(C& c)
{
    return dynamic_cast<D&>(c).name();
}

// C148. Use dynamic_cast to a pointer type when failure to find the require class is considered a valid alternative

// C149. Use unique_ptr or shared_ptr to avoid forgetting to delete objects created using new

} // namespace ================================================================

void test_c_140()
{
    bad::Derived d;
    bad::Base& b = d;

    std::cout << "b.multiply(10) : " << b.multiply(10) << std::endl;
    std::cout << "d.multiply(10) : " << d.multiply(10) << std::endl;
}

void test_c_146()
{
    CRTP::Base<Derived> d;

    d.interface();
    d.staticCall();
    {
        X x;
    }
    std::cout << "X created: " << CRTP::Counter<X>::objectCreated << std::endl;
    std::cout << "X alive: " << CRTP::Counter<X>::objectAlive << std::endl;

#if (0)
    Square s0(0, 0, 10, 10);
    std::shared_ptr<Square> p(dynamic_cast<Square*>(s0.clone()));
#else
    auto s0 = std::make_shared<Square>(0, 0, 10, 10);
    auto p = std::make_shared<Square>(*dynamic_cast<Square*>(s0->clone()));
#endif
    p->print();
}

void test_c_147()
{
    D d;
    std::cout << "className(d) : " << className(d) << std::endl;

//    libc++abi: terminating with uncaught exception of type std::bad_cast: std::bad_cast
//    className(e) :
    E e;
    std::cout << "className(e) : " << className(e) << std::endl;
}
