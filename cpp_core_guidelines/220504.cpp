#include <iostream>
#include <memory>

namespace { //=================================================================

// R.20 Use unique_ptr or shared_ptr to represent ownership

class A
{
};

void funcA()
{
    A a;
    A* pA{ new A };
    auto p0 = std::unique_ptr<A>{ new A };
    auto p1 = std::shared_ptr<A>{ new A };
    auto p2 = std::make_unique<A>();
    auto p3 = std::make_shared<A>();
}

// R.21 Prefer unique_ptr over shared_ptr unless you need to share ownership

// R.22 Use make_shared() to make shared_ptrs

void useMakeShared()
{
    // bad
    std::shared_ptr<A> p0{ new A };

    // good
    auto p1 = std::make_shared<A>();
}

// R.23 Use make_unique to make unique_ptrs

// R.24 Use std::weak_ptr to break cycles of shared_ptrs

class Bar;

class Foo
{
    std::shared_ptr<Bar> pBar;
public:
    explicit Foo(const std::shared_ptr<Bar>& p) : pBar(p)
    {
        std::cout << "Foo()" << std::endl;
    }
   ~Foo()
    {
        std::cout << "~Foo(), use_count : " << pBar.use_count() << std::endl;
    }
};

class Bar
{
    std::weak_ptr<Foo> pFoo;
public:
    explicit Bar(const std::weak_ptr<Foo>& p) : pFoo(p)
    {
        std::cout << "Bar(), use_count : " << pFoo.use_count() << std::endl;
    }
   ~Bar()
    {
        std::cout << "~Bar(), use_count : " << pFoo.use_count() << std::endl;
    }

    void work()
    {
        if (auto ref = pFoo.lock()) {
            std::cout << "Bar::work()" << std::endl;
        } else {
            std::cout << "pFoo expired..." << std::endl;
        }
    }
};

std::weak_ptr<int> wpInt;

void observe()
{
    std::cout << "wpInt.use_count : " << wpInt.use_count() << std::endl;

    if (auto spInt = wpInt.lock()) {
        std::cout << "int : " << *spInt.get() << std::endl;
    } else {
        std::cout << "wpInt expired..." << std::endl;
    }
}

// R.30 Take smart pointers as parameters only to explicitly express lifetime semantics

// R.31 If you have non-std smart pointers, follow the basic patter from std

} // namespace ================================================================

void test_r_24()
{
    {
        auto spInt = std::make_shared<int>(62);
        wpInt = spInt;
        observe();
    }
    observe();

    std::weak_ptr<Foo> wpFoo;

    auto spBar = std::make_shared<Bar>(wpFoo);
    auto spFoo = std::make_shared<Foo>(spBar);

    std::cout << "wpFoo = spFoo" << std::endl;
    wpFoo = spFoo;

    spBar->work();
}
