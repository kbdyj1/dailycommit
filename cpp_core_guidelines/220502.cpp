#include <iostream>
#include <memory>

namespace { //=================================================================

// R1. Manage resources automatically using resource handles and RAII(Resource Acquisition Is Initialization)

class A
{
public:
    A() { std::cout << "A()" << std::endl; }
   ~A() { std::cout << "~A()" << std::endl; }
};

void sendA(std::unique_ptr<A> a, std::string_view v)
{
    std::cout << "sendA(" << v << ")" << std::endl;
}

// R2. In interfaces, use raw pointers to denote indivisual objects (only)

// R3. A raw pointer (a T*) is non-owning

template <class T, class = std::enable_if<std::is_pointer<T>::value>>
using owner = T;

template <typename T>
class B
{
    // bad. unclear whether p and q are owning or not
    T* p;
    T* q;
};

template <typename T>
class C
{
    // owning
    owner<T*> p;

    // not owning
    T* q;
public:
    C(T* own, T* noOwn) : p(own), q(noOwn)
    {}
   ~C()
    {
        delete p;
    }
};

// R4. A raw reference (a T&) is non-owning

// R5. Prefer scoped object, don't heap-allocated unnecessary

// R6. Avoid non-const global variables

} // namespace ================================================================

void test_r_1()
{
    auto s = std::string("Hello, Qt");
    std::string_view v(s);
    sendA(std::unique_ptr<A>(new A), v);
}

void test_r_3()
{
    //bad
    int* p0 = new int{7}; // raw owning pointer

    //ok
    auto p1 = std::make_unique<int>(7);

    auto c = C<int>(p0, p1.get());
}

void test_r_4()
{
    // bad
    int& r = *new int(7);

    delete &r;
}
