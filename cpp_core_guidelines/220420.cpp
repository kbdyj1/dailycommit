#include <iostream>
#include <functional>
#include <memory>
#include "memory.h"

namespace { //=================================================================

// C83. For value-like types, consider providing a noexcept swap function

class A
{
    std::string str;

public:
    A() = default;
    A(const A&) = default;
    A& operator=(const A&) = default;
   ~A() = default;
    A(const std::string& s) : str(s)
    {}
    void swap(A& a) noexcept
    {
        std::swap(str, a.str);
    }

    std::string s() const
    {
        return str;
    }
};

class B
{
    int value;
    A a;
public:
    B(std::string s, int i)
        : a(s)
        , value(i)
    {}

    void swap(B& b) noexcept
    {
        std::swap(value, b.value);
        a.swap(b.a);
    }

    friend void print(const B&);
};

void swap(B& b0, B& b1)
{
    b0.swap(b1);
}

void print(const B& b)
{
    std::cout << "B(" << b.a.s() << ", " << b.value << ")" << std::endl;
}

// C84. A swap function must not fail

// C85. Make swap noexcept

// C86. Make == symmetric with respect to operand types and noexcept

namespace c86
{

struct X
{
    std::string name;
    int id;
};
bool operator==(const X& x0, const X& x1) noexcept
{
    return x0.name == x1.name && x0.id == x1.id;
}

class Y
{
    std::string name;
    int id;

    bool operator==(const Y& y) const {
        return name == y.name && id == y.id;
    }
};

} // c86

// C87. Beware of == on base classes

// C89. Make a hash noexcept

#if (0)
template <>
struct hash<My> {
    using result_type = size_t;
    using argument_type = My;

    size_t operator()(const My& x) const {
        size_t s = x.s.size();

        return std::hash<size_t>()(x.s.size()) ^ trim(x.s);
    }
};
#endif

// C90. Rely on constructors and assignment operators, not memset and memcpy

struct Base
{
public:
    virtual void update() = 0;
    std::shared_ptr<int> p;
};

struct Derived : public Base
{
    void update() override
    {}
};

void badInit(Derived& d)
{
    // type unsafe and overwritten the vtable
    memset(&d, 0, sizeof(Derived));
}

} // namespace ================================================================

void test_c_83()
{
    B b0("Qt5", 515);
    B b1("Qt6", 603);

    swap(b0, b1);

    std::cout << "b0 -> ";
    print(b0);

    std::cout << "b1 -> ";
    print(b1);
}
