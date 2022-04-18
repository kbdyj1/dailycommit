#include <iostream>

namespace { //=================================================================

// C60. Make copy assignment non-virtual, take the parameter by const&, and return by non-const&

class A
{
public:
    A& operator=(const A& a)
    {
        if (this != &a) {
            // some work
            std::cout << "=(const A&)" << std::endl;
        }
        return *this;
    }
    A& operator=(A&& a)
    {
        if (this != &a) {
            // some work
            std::cout << "=(A&&)" << std::endl;
        }
        return *this;
    }
};

A createA()
{
    return A{};
}

// C61. A copy operator should copy

template <typename T>
class B
{
    T* p;
    int size;

public:
    B() : p(nullptr), size(0)
    {}
    B(const B& b) : p(new T[b.size]), size(b.size)
    {}

   ~B()
    {
        delete [] p;
    }
    void modify()
    {
        std::cout << "B::modify()" << std::endl;
    }

    friend bool operator==(const B& b0, const B& b1);
};

template <typename T>
bool operator==(const B<T>& b0, const B<T>& b1)
{
    // range check

    return b0.size == b1.size;
}

// C62. Make copy assignment safe for self-assignment

// C63. Make move assignment non-virtual, take the parameter by &&, and return by non-const&

template <typename T>
class C
{
    T* p = nullptr;
    int size = 0;

public:
    C()
    {
        std::cout << "C()" << std::endl;
    }
    C(C&& c) : p(c.p), size(c.size)
    {
        std::cout << "C(C&&)" << std::endl;

        c.p = nullptr;
        c.size = 0;
    }
   ~C()
    {
        delete [] p;
    }
};

} // namespace ================================================================

void test_c_60()
{
    auto a = A{};
    auto b = A{};
    a = b;
    a = createA();
}

void test_c_64()
{
    C<int> c0;
    C<int> c1 = std::move(c0);
}
