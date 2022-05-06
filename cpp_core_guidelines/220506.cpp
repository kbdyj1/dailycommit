#include <iostream>
#include <memory>
#include <map>
#include <vector>

namespace { //=================================================================

struct Foo
{
    int n0;
    int n1;
    Foo(int a, int b) : n0(a), n1(b)
    {
        std::cout << "Foo(" << n0 << ", " << n1 << ")" << std::endl;
    }
   ~Foo()
    {
        std::cout << "~Foo(" << n0 << ", " << n1 << ")" << std::endl;
    }
};

// R.35 Take a shared_ptr<>& parameter to express that a function might reset the shared pointer

void changeFoo(std::shared_ptr<Foo>& s)
{
    s = std::make_shared<Foo>(3, 4);
}

// R.36 Take a const shared_ptr<>& parameter to express that it might retain a reference count to the object?

void retainFoo(const std::shared_ptr<Foo>& s)
{
    std::cout << "#1. retainFoo(const std::shared_ptr<Foo>&).use_count: " << s.use_count() << std::endl;

    auto copy = s;

    std::cout << "#2. retainFoo(const std::shared_ptr<Foo>&).use_count: " << s.use_count() << std::endl;
}

// R.37 Do not pass a pointer or reference obtained from an aliased smart pointer

#if (0)
std::shared_ptr<Foo> gFoo = std::make_shared<Foo>(10, 20);
#else
std::shared_ptr<Foo> gFoo;
#endif

void g()
{
    gFoo = std::make_shared<Foo>(5, 5);
}

void useFoo(Foo& foo)
{
    std::cout << "useFoo(" << foo.n0 << ", " << foo.n1 << ")" << std::endl;
}

void f(Foo& foo)
{
    g();
    useFoo(foo);
}

// ES.1 Preper the standard library to other libraries and to "handcrafted code"

// ES.2 Preper suitable abstraction to direct use of language features

// ES.3 Don't repeat yourself, avoid redundant code

// ES.5 Keep scopes small

// ES.6 Declare names in for-statement initializers and conditions to limit scope

// ES.7 Keep common and local names short, and keep uncommon and non-local names longer

template <typename T>
void print(std::ostream& os, const std::vector<T>& v)
{
    for (auto iter = std::begin(v); iter != std::end(v); iter++) {
        os << *iter << std::endl;
    }
}

// ES.8 Avoid similar-looking names

// ES.9 Avoid ALL CAPS names

} // namespace ================================================================

void test_weak_ptr()
{
    auto p0 = std::make_shared<Foo>(1, 2);
    std::shared_ptr<int> p1(p0, &p0->n0);
    std::shared_ptr<int> p2(p0, &p0->n1);

    std::cout << std::boolalpha
              << "p1 < p2" << (p1 < p2) << std::endl
              << "p2 < p1" << (p2 < p1) << std::endl
              << "p1.owner_before(p2) : " << p1.owner_before(p2) << std::endl
              << "p2.owner_before(p1) : " << p2.owner_before(p1) << std::endl;

    std::weak_ptr<int> w1(p1);
    std::weak_ptr<int> w2(p2);

    std::cout << "w1.owner_before(w2) : " << w1.owner_before(w2) << std::endl
              << "w2.owner_before(w1) : " << w2.owner_before(w1) << std::endl;
}

void test_r_35()
{
    auto p0 = std::make_shared<Foo>(1, 2);

    std::cout << "before changeFoo()" << std::endl;
    changeFoo(p0);
    std::cout << "after changeFoo()" << std::endl;
}

void test_r_36()
{
    auto p0 = std::make_shared<Foo>(1, 2);
    retainFoo(p0);
}

void test_r_37()
{
#if (0) // bad
    auto& foo = *gFoo;
#else
    auto copy = gFoo;
    auto& foo = *copy;
#endif
    std::cout << "before f(foo)" << std::endl;
    f(foo);
    std::cout << "after f(foo)" << std::endl;
}

void test_es_6()
{
    std::map<int, std::string> m;

    auto key = 6;
    auto value = std::string{"Qt"};
    if (auto result = m.insert({key, value}); result.second) {
        std::cout << "m.insert({key, value}) return first : " << result.first->first << std::endl;
    }
}

void test_es_7()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    print(std::cout, v);
}
