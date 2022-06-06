#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <memory>

using namespace std::literals;

namespace { //=================================================================

// T.40 Use function objects to pass operations to algorithms
//      in general, passing function objects gives better performance than passing pointers to functions

bool greater(double x, double y) { return x > y; }

template <typename T>
void print(const T& t)
{
    for (auto v : t) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

// T.41 Requires only essential properties in a template's concepts
//      keep interfaces simple and stable

#ifndef OS_MAC
#if (0)
void sort(std::sortable auto& s) // simple
#else
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& t)
{
    for (auto elem : t) {
        os << elem << " ";
    }
    os << std::endl;
    return os;
}

template <typename S>
concept Streamable = requires(S s, std::ostream& os) {
    os << s;
};

template <std::sortable S>
    requires Streamable<S>
void sort(S& s)
#endif
{
    std::cout << "enter sort(" << s << ")" << std::endl;
}
#endif

// T.42 Use template aliases to simplify notation and hide implementation details

template <typename T, size_t N>
class Matrix {
    using Iterator = typename std::vector<T>::iterator;
};

#if (0)
template <typename T>
using ValueType = container_traits<T>::value_type;

template <typename T>
void user(T& c)
{
    ValueType<T> x;
}
#endif

// T.43 Prefer using over typedef for defining aliases

#if (0)
typedef int (*pfunc)(int);
#else
using pfunc = int(*)(int);
#endif

template <typename T>
#if (0)
typedef int (*tfunc)(T);    // error: template declaration of ‘typedef’
#else
using tfunc = int(*)(T);    // OK
#endif

// T.44 Use function templates to deduce class template argument types (where feasible)

std::tuple<int, std::string, double> t0 = {1, "Hello, Qt", 3.14};   // explicit

auto t1 = std::make_tuple(1, "Hello, Qt"s, 3.14);   // deduced

std::tuple t2 = {2, "Hello, Qt6"s, 180.0};  // c++17. template parameter deduction for constructors

// T.46 Require template arguments to be at least semiregular
//  semiregular requires default constructible

class A
{
public:
    explicit A(int);
    A(const A&);
    A& operator=(const A&);
    A(A&&) noexcept;
    A& operator=(A&&) noexcept;
    ~A();
};

// T.47 Avoid highly visible unconstrained templates with common name

namespace t47 {
namespace bad {

struct S { int elem; };

template <typename T0, typename T1>
bool operator==(T0, T1)
{
    std::cout << "bad::operator==(T0,T1)" << std::endl;
    return true;
}

} //bad

bool operator==(int, bad::S)
{
    std::cout << "operator==(int,bad::S)" << std::endl;
    return true;
}

void test()
{
    t47::bad::S s{1};
    auto v = std::vector<int>(10);

    bool res0 = 1 == s;
    bool res1 = v.size() == s;
}

} //t47

// T.48 If your compiler does not support concepts, fake them with enable_if

template <typename T>
std::enable_if_t<std::is_integral_v<T>>
f(T v)
{
    std::cout << "f.is_integral(" << v << ")" << std::endl;
}

// T.49 Where possible, avoid type-erasure

struct IFunctor
{
    virtual int operator()(int) = 0;
};
template <typename Func>
struct FunctorImpl : public IFunctor
{
    Func func;
    FunctorImpl(Func f) : func(f)
    {}
    int operator()(int arg) override
    {
        return func(arg);
    }
};

template <typename Func>
IFunctor* makeFunctor(Func func)
{
    return new FunctorImpl<Func>(func);
}

int func0(int arg)
{
    std::cout << "func0(" << arg << ") -> ";
    return arg;
}

class Functor
{
    struct Interface {
        virtual int operator()(int) = 0;
    };
    template <typename Func>
    struct Impl : public Interface {
        Impl(Func f) : func(f)
        {}
        int operator()(int arg) override
        {
            std::cout << "Functor(" << arg << ") -> ";
            return func(arg);
        }
        Func func;
    };

public:
    template <typename Func>
    Functor(Func f) : inf(new Impl<Func>(f))
    {}
    int operator()(int arg)
    {
        std::cout << "Functor::operator()(" << arg << ") -> ";
        return (*inf)(arg);
    }
    std::shared_ptr<Interface> inf;
};

} //===========================================================================

void test_t_40()
{
    auto v = std::vector{ 5, 4, 1, 2, 3 };

    auto v0 = v;
    std::sort(std::begin(v0), std::end(v0), greater);
    print(v0);

    auto v1 = v;
    std::sort(std::begin(v1), std::end(v1), [](double x, double y){ return x > y; });
    print(v1);

    auto v2 = v;
    std::sort(std::begin(v2), std::end(v2), std::greater());
    print(v2);
}

void test_t_47()
{
    t47::test();
}

void test_t_48()
{
    f(0);
    //f("Hello, Qt"s);
}

void test_t_49()
{
    {
        std::map<int, IFunctor*> table;
        table[1] = makeFunctor(func0);
        table[2] = makeFunctor([](int arg) -> int {
            std::cout << "[](" << arg << ") -> ";
            return arg;
        });

        std::cout << "table[1](2) : " << (*table[1])(2) << std::endl;
        std::cout << "table[2](2) : " << (*table[2])(2) << std::endl;
    }
    {
        std::map<int, Functor> table{
            {1, func0},
            {2, [](int arg) -> int {
                    std::cout << "Functor.[](" << arg << ") -> ";
                    return arg;
                }
            }
        };
    }
}
