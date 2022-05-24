#include <iostream>
#include <memory>

namespace { //=================================================================

// Con.1 By default, make objects immutable

// Con.2 By default, make member functions const

namespace con2
{

void f(int* p)
{
    std::cout << "older f(int*)" << std::endl;
}

} //con2

void f(const int* p)
{
    std::cout << "wrapper f(const int*)" << std::endl;
    con2::f(const_cast<int*>(p));
}

class Date
{
public:
    const std::string& stringRef() const
    {
        if (cache == "")
            computeStringRep();
        return cache;
    }

private:
    void computeStringRep() const
    {
        cache = "2022/05/25";
    }
    mutable std::string cache;
};

// Con.3 By default, pass pointers and references to consts

// Con.4 Use const to define objects with values that do not change after construction

// Con.5 Use constexpr for values that can be computed at compile time

constexpr int g(int x)
{
    return x;
}

double d0 = g(0);
const double d1 = g(1);
constexpr double d2 = g(2);

// T.1 Use templates to raise the level of abstraction of code

// T.2 Use templates to express algorithms that apply to many argument types

// T.3 Use templates to express containers and ranges

// T.4 Use templates to express syntax tree manipulation ???

// T.5 Combine generic and OO techniques to amplify their strengths, not their costs
//      Use static polymorphism to implement dynamically polymorphic interfaces

class Object
{
public:
    template <typename T>
    Object(T&& obj)
        : concept_(std::make_shared<ConcretCommand<T>>(std::forward<T>(obj)))
    {}
    int getId() const {
        return concept_->getId();
    }

private:
    struct Command {
        virtual ~Command() {}
        virtual int getId() const = 0;
    };
    template <typename T>
    struct ConcretCommand final : Command {
        ConcretCommand(T&& obj) noexcept : object_(std::forward<T>(obj))
        {}
        int getId() const final { return object_.getId(); }
    private:
        T object_;
    };
    std::shared_ptr<Command> concept_;
};

class A {
public:
    int getId() const { return 1; }
};
struct B {
    int getId() const { return 2; }
};

} //===========================================================================

void test_con_2()
{
    int a = 0;
    f(&a);
}

void test_t_5()
{
    auto o0 = Object(A{});
    auto o1 = Object(B{});

    std::cout << "o0.getId() : " << o0.getId() << std::endl;
    std::cout << "o1.getId() : " << o1.getId() << std::endl;
}
