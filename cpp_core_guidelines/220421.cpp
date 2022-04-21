#include <iostream>
#include <vector>

namespace { //=================================================================

// C100. Follow the STL when defining a container

template <typename T>
class SortedVector
{
    std::vector<T> container;

public:
    using value_type = T;

    SortedVector() = default;
    SortedVector(std::initializer_list<T> l) : container(l)
    {}
    SortedVector(const SortedVector& v) : container(v.container)
    {
        std::cout << "SortedVector(const SortedVector&)" << std::endl;
    }
    SortedVector(SortedVector&& v) : container(std::move(v.container))
    {
        std::cout << "SortedVector(SortedVector&&)" << std::endl;
    }
    SortedVector& operator=(const SortedVector& v)
    {
        std::cout << "SortedVector& operator=(const SortedVector&)" << std::endl;
        container = v.container;
    }
    SortedVector& operator=(SortedVector&& v)
    {
        std::cout << "SortedVector& operator=(const SortedVector&& v)" << std::endl;
        container = std::move(v.container);
    }
   ~SortedVector() = default;

    SortedVector(const std::vector<T>& v) : container(v)
    {
        std::cout << "SortedVector(const std::vector<T>&)" << std::endl;
    }
    SortedVector(std::vector<T>&& v) : container(std::move(v))
    {
        std::cout << "SortedVector(std::vector<T>&&)" << std::endl;
    }

    const T& operator[](int i) const { return container[i]; }

    void push_back(const T& elem)
    {
        container.push_back(elem);
    }
    void push_back(T&& elem)
    {
        container.push_back(std::move(elem));
    }
    void print() const
    {
        for (auto s : container) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    bool equal(const SortedVector<T>& other) const
    {
        return container == other.container;
    }
};

template <typename T>
bool operator==(const SortedVector<T>& v0, const SortedVector<T>& v1)
{
    return v0.equal(v1);
}

template <typename T>
bool operator!=(const SortedVector<T>& v0, const SortedVector<T>& v1)
{
    return !v0.equal(v1);
}

// C101. Give a container value semantics

void f(const SortedVector<std::string>& v0)
{
    SortedVector<std::string> v1{v0};
    if (v0 != v1) {
        std::cout << "Behavior against reason and logic" << std::endl;
    } else {
        v0.print();
    }
}

// C102. Give a container move operations

SortedVector<int> g()
{
    std::vector<int> v{1, 2, 3, 4, 5};
    SortedVector<int> ret = v;
    return ret;
}

// C103. Give a container an initializer list constructor

// C104. Give a container a default constructor that sets it to empty

// C109. If a resource handle has pointer sematics, provide * and ->

} // namespace ================================================================

void test_c_101()
{
    SortedVector<std::string> v{"Hello", "Qt"};

    f(v);
}

void test_c_102()
{
    SortedVector<int> v = g();
    v.print();
}

void test_c_103()
{
    SortedVector<int> v{1, 2, 3, 4, 5};
}

void test_c_104()
{
    std::vector<SortedVector<std::string>> v(100);
}
