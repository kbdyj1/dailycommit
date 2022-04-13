#include <iostream>

namespace { //=================================================================

// C30. Define a destructor if a class needs an explicit action at object destruction

template <typename A>
struct FinalAction
{
    A act;
    FinalAction(A a) : act(a)
    {}
    ~FinalAction() {
        act();
    }
};

template <typename A>
FinalAction<A> finally(A act)
{
    return FinalAction<A>{act};
}

class B
{
public:
    void operator()(){ std::cout << "Qt6" << std::endl; }
};

// C31. All resources acquired by a class must be released by the class's destructor

// C32. If a class has a raw pointer (T*) or reference (T&), consider whether it might be owning

// C33. If a class has an owning pointer member, define a destructor

// C35. A base class destructor should be either public and virtual, or protected and non-virtual

class C
{
protected:
    ~C()
    {
        std::cout << "~C() protected." << std::endl;
    }
};
class D : public C
{
protected:
    ~D()
    {
        std::cout << "~D()" << std::endl;
    }
};

class E : public D
{
public:
    ~E()
    {
        std::cout << "~E()" << std::endl;
    }
};
} // namespace ================================================================

void test_c_30()
{
    std::cout << "Begin..." << std::endl;
    {
        auto act0 = finally([]{ std::cout << "Exit !!!" << std::endl; });
        auto act1 = finally(B{});
    }
    std::cout << "End..." << std::endl;
}

void test_c_35()
{
    E e;
}
