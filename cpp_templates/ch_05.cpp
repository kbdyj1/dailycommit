//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.05 Foundation
//
//  22/09/22
//=============================================================================

#include <vector>
#include <iostream>

namespace { //=================================================================

int x[] = { 1, 2, 3, 4 };

template <typename C>
void printContainer(const C& container)
{
    typename C::const_iterator iter;
    typename C::const_iterator end(container.end());
    for (iter=std::begin(container); iter!=end; iter++) {
        std::cout << *iter << ' ';
    }
    std::cout << "\n";
}

void test_print_container()
{
    auto v = std::vector<int>{ 1, 2, 3, 4, 5 };
    printContainer(v);
}

#define BUILTIN_TYPE_INITIALIZATION

template <typename T>
void foo()
{
#if defined(BUILTIN_TYPE_INITIALIZATION)
    T x{};  //0
#else
    T x;    //21845
#endif

    std::cout << "foo() x: " << x << "\n";
}

template <typename T>
class MyClass {
#if defined(BUILTIN_TYPE_INITIALIZATION)
    T x{};
#else
    T x;
#endif
public:
    MyClass() {
        std::cout << "MyClass(x:" << x << ")\n";
    }
};

template <typename T>
void bar(T p = T{})
{
    std::cout << "bar(" << p << ")\n";
}

void test_builtin_type_initialization()
{
    foo<int>();
    auto my = MyClass<int>{};
    bar<int>(4);
}

// 5.3
template <typename T>
class Base {
public:
    void bar() {
        std::cout << "Base::bar()\n";
    }
};

template <typename T>
class Derived : public Base<T> {
public:
    void foo() {
#if (0)
        this->bar();
#else
        Base<T>::bar();
#endif
    }
};

// 5.4
template <typename T, int N, int M>
bool less(T(&a)[N], T(&b)[M])
{
    std::cout << "less([" << N << "], [" << M << "])\n";

    for (auto i=0; i<N && i<M; i++) {
        if (a[i] < b[i])
            return true;
        if (b[i] < a[i])
            return false;
    }
    return N < M;
}

void test_less()
{
    int x[] = { 1, 2, 3 };
    int y[] = { 1, 2, 3, 4, 5 };

    std::cout << less(x, y) << "\n";
    std::cout << less("Hello", "Qt") << "\n";
}

template <typename T>
struct A;

template <typename T, std::size_t SIZE>
struct A<T[SIZE]> {
    static void print() { std::cout << "A<[SIZE]>\n"; }
};

template <typename T, std::size_t SIZE>
struct A<T(&)[SIZE]> {
    static void print() { std::cout << "A<T(&)[SIZE]>\n"; }
};

template <typename T>
struct A<T[]> {
    static void print() { std::cout << "A<T[]>\n"; }
};

template <typename T>
struct A<T(&)[]> {
    static void print() { std::cout << "A<T(&)[]>\n"; }
};

template <typename T>
struct A<T*> {
    static void print() { std::cout << "A<T*>\n"; }
};

template <typename T0, typename T1, typename T2>
void foo(int a0[8], int a1[], int (&a2)[40], int (&a3)[], T0 x, T1& y, T2&& z)
{
    A<decltype (a0)>::print();  //A<T*>
    A<decltype (a1)>::print();  //A<T*>
    A<decltype (a2)>::print();  //A<T(&)[SIZE]>
    A<decltype (a3)>::print();  //A<T(&)[]>
    A<decltype (x)>::print();   //A<T*>
    A<decltype (y)>::print();   //A<T(&)[]>
    A<decltype (z)>::print();   //A<T(&)[]>
}

void test_array_specialization()
{
    int a[40];
    A<decltype (a)>::print();

    extern int x[];
    A<decltype (x)>::print();

    foo(a, a, a, x, x, x, x);
}

} //namespace =================================================================

void test_ch_05()
{
    std::cout << std::boolalpha;

#if (0) // done
    test_print_container();
    test_builtin_type_initialization();
    test_less();
#endif

    test_array_specialization();
}
