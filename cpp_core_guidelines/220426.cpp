#include <iostream>

namespace { //=================================================================

// C138. Create an overload set for a derived class and its bases with using

class B
{
public:
    virtual int f(int i) { std::cout << "int f(int i)" << std::endl; return i; }
    virtual double f(double i) { std::cout << "int B::f(double i)" << std::endl; return i; }
    virtual ~B() = default;
};

namespace bad
{

class D : public B
{
public:
    int f(int i) override { std::cout << "int D::f(int i)" << std::endl; return i+1; }
};

} // bad

namespace good
{

class D : public B
{
public:
    int f(int i) override { std::cout << "int D::f(int i)" << std::endl; return i+1; }
    using B::f;
};

} // good

template <class... Ts>
struct Overloader : Ts... {
    using Ts::operator()...;
};

} // namespace ================================================================

void test_c_138()
{
#if (0)
    bad::D d;
#else
    good::D d;
#endif
    std::cout << d.f(2) << std::endl;
    std::cout << d.f(2.3) << std::endl;
}
