#include <iostream>

namespace { //=================================================================

// C152. Never assign a pointer to an array of derived class objects to a pointer to its base

struct A
{
    int a;
};
struct B : public A
{
    int b;
};

void useB(B* p)
{
    std::cout << "B.a: " << p->a << ", B.b: " << p->b << std::endl;
}

// C153. Prefer virtual function to casting

// C160. Define operators primarily to mimic conventional usage

class C
{
public:
    C& operator=(const C&) = default;
    friend bool operator==(const C&, const C&);
};

// C161. Use non-member functions for symmetric operators

// C162. Overload operations that are roughly equivalent

// C163. Overload only for operations that are roughly equivalent

// C164. Avoid implicit conversion operators

namespace bad
{

class S
{
    std::string str;
public:
    S(const std::string& s) : str(s)
    {}
    operator char*() { return str.data(); }
};

} // bad

namespace good
{

class S
{
    std::string str;
public:
    S(const std::string& s) : str(s)
    {}
    explicit operator char*() { return str.data(); }
};

} // good

namespace N
{

struct X
{
    int x;
};

void swap(N::X& x0, N::X& x1)
{
    std::cout << "swap(N::X& x0, N::X& x1)" << std::endl;
}

void f1(N::X& x0, N::X& x1)
{
    std::cout << "f1(N::X& x0, N::X& x1)" << std::endl;
    std::swap(x0, x1);
}

void f2(N::X& x0, N::X& x1)
{
    std::cout << "f2(N::X& x0, N::X& x1)" << std::endl;
    swap(x0, x1);
}

void f3(N::X& x0, N::X& x1)
{
    std::cout << "f3(N::X& x0, N::X& x1)" << std::endl;
    using std::swap;

    swap(x0, x1);
}

} // N

// C165. Use using for customize points

} // namespace ================================================================

void test_c_152()
{
    B bs[] = {{0, 1}, {2, 3}, {4, 5}};
    A* p = bs;
    p[1].a = 6;
    useB(bs);
}

void test_c_164()
{
    bad::S b("Bad");
    char *s0 = b;
    std::cout << "s0: " << s0 << std::endl;

    good::S g("Good");
    char *s1 = static_cast<char*>(g);
    std::cout << "s1: " << s1 << std::endl;
}

void test_c_165()
{
    N::X x0 { 1 };
    N::X x1 { 2 };

    f3(x0, x1);
}
