#include <iostream>
#include <memory>

namespace { //=================================================================

// C130. For making deep copies of polymorphic classes prefer a virtual clone
//      function instead of public copy construction/assignment

class A
{
public:
    A() = default;
    virtual ~A() = default;
    virtual A* clone() const = 0;

protected:
    A(const A&) = default;
    A& operator=(const A&) = default;
    A(A&&) = default;
    A& operator=(A&&) = default;
};

class B : public A
{
    std::string name;

public:
    B() = default;
    B(const B& other) : name(other.name)
    {
        std::cout << "name :" << name << std::endl;
    }
    B& operator=(const B& other)
    {
        if (this != &other) {
            name = other.name;
        }
        return *this;
    }
    B(B&& other) : name(std::move(other.name))
    {}
    B& operator=(B&& other)
    {
        if (this != &other) {
            name = std::move(other.name);
        }
        return *this;
    }
    B* clone() const override
    {
        return new B(*this);
    }

    void setName(const std::string &str)
    {
        name = str;
    }
    std::string s() const
    {
        return name;
    }
};

// C131. Avoid trivial getters and setters

// C132. Don't make function virtual without reason

template <typename T>
class Vector
{
    T* mElem;
    int mSize;

public:
    virtual int size() const { return mSize; } // bad. redundant virtual function
};

// C133. Avoid protected data

// C134. Ensure all non-const data members have the same access level
//      two kind of data members
//      1. Ones that don't participate in the object's invariant. (public or protected)
//      2. Ones that do participate in the object's invariant. (private or const)

// C135. Use multiple inheritance to represent multiple distinct interfaces

// C136. Use multiple inheritance to represent the union of implementation attributes

// C137. Use virtual bases to avoid overly general base classes

struct Interface // interface inheritance
{
    virtual void description() = 0;
};

class Utility // implementation inheritance
{
    void utility1()
    {
        std::cout << "utility1(" << mX << ", " << mY << ")" << std::endl;
    }
    virtual void utility2() = 0;

public:
    int mX;
    int mY;
};

class Derived1 : public Interface, virtual protected Utility
{
public:
    void description() override
    {
        std::cout << "Derived1::description()" << std::endl;
    }
    void utility2() override
    {
        std::cout << "Derived1::utility2()" << std::endl;
    }
};

class Derived2 : public Interface, virtual protected Utility
{
public:
    void description() override
    {
        std::cout << "Derived2::description()" << std::endl;
    }
    void utility2() override
    {
        std::cout << "Derived2::utility2()" << std::endl;
    }
};

} // namespace ================================================================

void test_c_130()
{
    auto b0 = std::unique_ptr<B>(new B);
    b0->setName("Hello, Qt");

    auto b1 = b0->clone();
    std::cout << "b1.name : " << b1->s() << std::endl;
}

void test_c_137()
{
    auto d1 = std::unique_ptr<Derived1>(new Derived1);
    d1->description();
    d1->utility2();

    auto d2 = std::unique_ptr<Derived2>(new Derived2);
    d2->description();
    d2->utility2();
}
