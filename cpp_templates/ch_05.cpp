//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.05 Foundation
//
//  22/09/22
//=============================================================================

#include <vector>
#include <iostream>
#include <deque>
#include <cassert>
#include <string>
#include <bitset>
#include <limits>

using namespace std::string_literals;

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

template <typename T, template<typename Elem, typename = std::allocator<Elem>> class C = std::deque>
class Stack {
    C<T> container;

public:
    void push(const T& elem) {
        container.push_back(elem);
    }
    void pop() {
        assert(!container.empty());

        container.pop();
    }
    const T& top() const {
        assert(!container.empty());

        return container.back();
    }
    bool empty() const {
        return container.empty();
    }

    void print() {
        for (auto iter=std::begin(container); iter!=std::end(container); iter++) {
            std::cout << *iter << " ";
        }
    }

    template <typename T2, template <typename Elem2, typename = std::allocator<Elem2>> class C2>
    Stack& operator=(const Stack<T2, C2>& other);

    template <typename, template<typename, typename> class> friend class Stack;
};

template <typename T, template<typename, typename> class C>
template <typename T2, template <typename, typename> class C2>
Stack<T,C>& Stack<T,C>::operator=(const Stack<T2,C2>& other)
{
    container.clear();
    container.insert(container.begin(), other.container.begin(), other.container.end());

    return *this;
}

void test_stack()
{
    auto iStack = Stack<int>{};
    iStack.push(10);
    iStack.push(20);

    auto fStack = Stack<float>{};
    fStack.push(1.4);
    fStack.push(3.4);

    iStack = fStack;

    iStack.print();
}

class BoolString {
    std::string value;

public:
    BoolString(const std::string& s) : value(s)
    {}

    template<typename T = std::string>
    T get() const {
        return value;
    }
};

template<>
inline bool BoolString::get<bool>() const {
    return value == "true" || value == "1" || value == "on";
}

void test_bool_string()
{
    auto s0 = BoolString("Hello"s);
    std::cout << s0.get() << "\n";

    auto s1 = BoolString("on");
    std::cout << s1.get() << "\n";
    std::cout << s1.get<bool>() << "\n";
}

template <unsigned long N>
void printBitSet(const std::bitset<N>& bs)
{
#if (1)
    std::cout << bs.template to_string<char, std::char_traits<char>, std::allocator<char>>() << "\n";
#else
    std::cout << bs.to_string() << "\n";
#endif
}

void test_bitset()
{
    auto bitset = std::bitset<4>{0xa};

    printBitSet(bitset);
}

void test_generic_lambda() //c++14
{
    auto func = [](auto v0, auto v1) {
        return v0 + v1;
    };

    std::cout << func('a', 10) << "\n";
    std::cout << func(20u, 34.5) << "\n";
}

template <typename T>
constexpr T pi{ 3.1415926535897932385 };

template <typename T>
class B {
public:
    static constexpr int max = 1024;
};

template <typename T>
int myMax = B<T>::max;

template <typename T>
constexpr bool isSigned = std::numeric_limits<T>::is_signed;

void test_variable_template()
{
    std::cout << pi<double> << "\n";
    std::cout << pi<float> << "\n";

    std::cout << isSigned<char> << "\n";
    std::cout << isSigned<unsigned long> << "\n";
}

} //namespace =================================================================

void test_ch_05()
{
    std::cout << std::boolalpha;

#if (0) // done
    test_print_container();
    test_builtin_type_initialization();
    test_less();
    test_array_specialization();
    test_bool_string();
    test_bitset();
    test_generic_lambda();
    test_variable_template();
#endif

    test_stack();
}
