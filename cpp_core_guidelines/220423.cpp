#include <iostream>
#include <memory>
#include <span>

namespace { //=================================================================

// Class inheritance.
// 1. implementation inheritance
// 2. interface inheritance

// C120. Use class hierarchies to represent concepts with inherent hierarchical structure (only)

namespace good
{

class Drawable {
public:
    virtual void render() const = 0;
};

class AbstractButton : public Drawable {
public:
    virtual void onClicked() = 0;
};

class PushButton : public AbstractButton {
public:
    void render() const override
    {}
    void onClicked() override
    {}
};

class My {
public:
    // pure virtual functions

    virtual ~My() = default;
};

}

namespace bad
{

// Do not represent non-hierarchical domain concepts as class inheritances
template <typename T>
class Container {
public:
    // list
    virtual T& get() = 0;
    virtual void put(T&) = 0;
    virtual void insert(int position) = 0;

    // vector
    virtual T& operator[](int) = 0;
    virtual void sort() = 0;

    // tree
    virtual void balance() = 0;
};

class Goof {
public:
    // pure virtual functions ...

    // no virtual destructor
};

class Derived : public Goof {
    std::string str;
public:
    Derived(std::string s) : str(s)
    {}
};

void use()
{
    auto p = std::unique_ptr<Goof>(new Derived{"Hello, Qt6"});
}

}

// C121. If a base class is used as an interface, make it a pure abstract class

// C122. Use abstract classes as interfaces when complete separation of interface and implementation is needed

struct Device {
    virtual ~Device() = default;
    virtual void write(std::span<const char> out) = 0;
    virtual void read(std::span<char> in) = 0;
};

class D1 : public Device {
public:
    void write(std::span<const char> out) override
    {}
    void read(std::span<char> in) override
    {}
};

// C126. An abstract class typically doesn't need a user-written constructor
//      an abstract class typically does not have any data for a constructor to initialize

struct Point {
    int x;
    int y;
};

class Shape {
public:
    virtual Point center() const = 0;
    virtual void move(Point) = 0;
    virtual ~Shape()
    {}
};

class Circle : public Shape {
    Point pt;
    float rad;
public:
    Circle(Point p, float r)
        : pt(p)
        , rad(r)
    {}
    Point center() const override
    {
        return pt;
    }
    void move(Point p) override
    {
        pt.x += p.x;
        pt.y += p.y;
    }
   ~Circle() override
    {}
};

// C127. A class with a virtual function should have a virtual or protected destructor

struct B {
    virtual void f() = 0;

    // default to public non-virtual
};

struct D : public B {
    std::string str{"default"};

    void f() override
    {
        std::cout << "D(" << str << ")" << std::endl;
    }
};

// C128. Virtual functions should specify exactly one of virtual, override, or final

struct Bad {
    void f1(int);
    virtual void f2(int) const;
    virtual void f3(int);
};

struct BadDerived : Bad {
    void f1(int);
    void f2(int) const;
    void f3(double);
};

} // namespace ================================================================

void test_c_127()
{
    std::unique_ptr<B> p = std::make_unique<D>();
    p->f();
    // B::~B() leak the string
}
