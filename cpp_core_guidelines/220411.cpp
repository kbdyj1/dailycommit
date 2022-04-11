#include <iostream>

namespace { //=================================================================

// C1. Organize related data into structures (structs or classes)

// C2. Use class if the class has an invariant; use struct if the data members can vary independently

// C3. Represent the distinction between an interface and an implementation using a class

// C4. Make a function a member only if it needs direct access to the representation of a class

class Date
{
    // relatively small interface
};

// helper
Date nextWeekday(const Date&);
bool operator==(const Date& d0, const Date& d1);

// C5. Place helper functions in the same namespace as the class they support

// C7. Don't define a class or enum and declare a variable of its type in the same statement

struct A
{
    int a;
    int b;
} a { 1, 2 }; // Bad. type + object

struct B
{
    int a;
    int b;
};
B b { 3, 4 }; // Good

// C8. Use class rather than struct if any member is non-public

// C9. Minimize exposure of members

class C
{
public:
    int foo(int x) { check(x); return bar(x); }
protected:
    int bar(int x) { return x; }
    bool check(int x) { return true; }
};

class D : public C
{
public:
    int mem(int x, int y)
    {
        return bar(x + y);
    }
};

void user(C& c)
{
    std::cout << "c.foo(1) : " << c.foo(1) << std::endl;
    //std::cout << "c.bar(2) : " << c.bar(2) << std::endl;
}

} // namespace ================================================================

void test_c_7()
{
    std::cout << "a.a : " << a.a << ", a.b : " << a.b << std::endl;
    std::cout << "b.a : " << b.a << ", b.b : " << b.b << std::endl;
}

void test_c_9()
{
    C c;

    user(c);
}
