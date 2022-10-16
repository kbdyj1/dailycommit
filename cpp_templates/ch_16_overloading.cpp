#include <iostream>

namespace { //=================================================================

namespace _1 {

template <typename T>
int f(T)
{
    return 0;
}

template <typename T>
int f(T*)
{
    return 1;
}

void test()
{
    std::cout << "f<int*>(): " << f<int*>((int*)nullptr) << "\n";
    std::cout << "f<int>(): " << f<int>((int*)nullptr) << "\n";

    std::cout << "f(0): " << f(0) << "\n";
    std::cout << "f(nullptr): " << f(nullptr) << "\n";
    std::cout << "f((int*)nullptr): " << f((int*)nullptr) << "\n";
}

} //_1 --------------------------------------------------------------

namespace _2 { //signature

template <typename T1, typename T2>
void f0(T1, T2)
{}

template <typename T1, typename T2>
void f0(T2, T1)
{}

template <typename T>
long f1(T)
{
    return 0L;
}

template <typename T>
char f1(T)
{
    return 0;
}

void test()
{
#if (0)
    //call to ** is ambiguous
    f0<char, char>('a', 'b');
    auto ret = f1(10);
#endif
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T>
void f(T*, const T* = nullptr, ...)
{}

template <typename T>
void f(const T*, T*, T* = nullptr)
{}

void test()
{
    auto i = (int)0;
    auto*p = &i;
#if (0) //ambiguous
    f(p, p);
#endif
}

} //_3 --------------------------------------------------------------

namespace _4 {

template <typename T>
std::string f(T&)
{
    return "f()";
}

std::string f(const int&)
{
    return "f(const int&)";
}

void test()
{
    int x = 0;
    std::cout << f(x) << "\n";

    const int y = 0;
    std::cout << f(y) << "\n";
}

} //_4 --------------------------------------------------------------

namespace _5 {

class A {
public:
    A() = default;
    A(const A&) { std::cout << "A(const A&)\n"; }
    A(A&&) { std::cout << "A(A&&)\n"; }

    template<typename T>
    A(T&&) { std::cout << "A(T&&)\n"; }
};

void test()
{
    auto a0 = A{};
    auto a1{a0};
    auto a2{std::move(a0)};

    const auto a3 = A{};
    auto a4{a3};
    auto a5{std::move(a3)};
}

} //_5 --------------------------------------------------------------

namespace _6 {

template <typename T>
int f(T*)
{
    return 1;
}

template <typename... Ts>
int f(Ts...)
{
    return 2;
}

template <typename... Ts>
int f(Ts*...)
{
    return 3;
}

template <typename... Ts>
class Tuple
{};

template <typename T>
int g(Tuple<T*>)
{
    return 1;
}

template <typename... Ts>
int g(Tuple<Ts...>)
{
    return 2;
}

template <typename... Ts>
int g(Tuple<Ts*...>)
{
    return 3;
}

void test()
{
    std::cout << "f(0,0.0): " << f(0, 0.0) << "\n";
    std::cout << "f((int*)nullptr, (double*)nullptr): " << f((int*)nullptr, (double*)nullptr) << "\n";
    std::cout << "f((int*)nullptr): " << f((int*)nullptr) << "\n";


    std::cout << g(Tuple<int, double>{}) << "\n";
    std::cout << g(Tuple<int*, double*>{}) << "\n";
    std::cout << g(Tuple<int*>{}) << "\n";
}

} //_6 --------------------------------------------------------------

namespace _7 { //full specialization

template <typename T>
class A {
public:
    void info() { std::cout << "A<T>()\n"; }
};

template <>
class A<void> {
public:
    void info() { std::cout << "A<void>()\n"; }
};

template <typename T>
class Types {
public:
    using I = int;
};

template <typename T, typename U = typename Types<T>::I>
class B
{};

template <>
class B<void> {
public:
    void f()
    {}
};

template <>
class B<char, char>
{};

#if (0)
template <>
class B<char, 0>    //template argument for template type parameter must be a type
{};
#endif

void test()
{
    B<int>* pi;
    B<int> b0;
    B<void>* pv;
}

} //_7 --------------------------------------------------------------

namespace _8 {

template <typename T>
class Out {
public:
    template<typename U>
    class In {
    };
};

template <>
class Out<void> {
    template<typename U>
    class In {
        static int count;
    };
};

template <typename U>
int Out<void>::In<U>::count = 1;

template <typename T>
class Invalid
{};

Invalid<double> x0;

#if (0) //has already been instantiated
template <>
class Invalid<double>;
#endif

} //_8 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_16_overloading()
{
#if (0) //done
    _1::test();
    _4::test();
    _5::test();
#endif

    _6::test();
}
