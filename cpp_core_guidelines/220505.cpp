#include <iostream>
#include <memory>

namespace { //=================================================================

// R.32 Take a unique_ptr<widget> parameter to express that a function assumes ownership of a widget

class A
{
public:
    A()
    {
        std::cout << "A()" << std::endl;
    }
   ~A()
    {
        std::cout << "~A()" << std::endl;
    }
    void print()
    {
        std::cout << "A::print()" << std::endl;
    }
};

void takeOwnership(std::unique_ptr<A> a)
{
    std::cout << "takeOwnership(std::unique_ptr<A>)" << std::endl;
}

void whatYouWant(const std::unique_ptr<A>& a) // -> A* or A&
{
    std::cout << "whatYouWant(const std::unique_ptr<A>&)" << std::endl;
}

void justUsePtr(A* a)
{
    a->print();
}

// R.33 Take a unique_ptr<widget>& parameter to express that a function reseats the widget

class B
{
    std::string message;
public:
    B(const std::string& s) : message(s)
    {
        std::cout << "B(" << message << ")" << std::endl;
    }
   ~B()
    {
        std::cout << "~B(" << message << ")" << std::endl;
    }
};

void reseat(std::unique_ptr<B>& a)
{
    a = std::make_unique<B>("Hello, Qt6");
}

// R.34 Take a shared_ptr<widget> parameter to express shared ownership

class C
{
    std::shared_ptr<A> mA;
public:
    C(std::shared_ptr<A> a) : mA(a)
    {
        std::cout << "C(std::shared_ptr<A>).use_count : " << a.use_count() << std::endl;
    }
};

} // namespace ================================================================

void test_r_32()
{
    auto a = std::make_unique<A>();
    auto pA = a.get();

    std::cout << "call justUsePtr(A*)" << std::endl;
    justUsePtr(pA);

    std::cout << "call whatYouWant(a)" << std::endl;
    whatYouWant(a);

    std::cout << "call takeOwnership(std::move(a))" << std::endl;
    takeOwnership(std::move(a));

    std::cout << "end test_r_32()" << std::endl;
}

void test_r_33()
{
    auto b = std::make_unique<B>("c++");
    std::cout << "before reseat(b)" << std::endl;
    reseat(b);
    std::cout << "after reseat(b)" << std::endl;
}

void test_r_34()
{
    auto a = std::make_shared<A>();
    {
        C c(a);
        std::cout << "a.use_count : " << a.use_count() << std::endl;
    }
    std::cout << "a.use_count : " << a.use_count() << std::endl;
}
