#include <iostream>

namespace { //=================================================================

namespace _1 { //full specialization

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

} //_1 --------------------------------------------------------------

namespace _2 {

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

} //_2 --------------------------------------------------------------

namespace _3 { //function

template <typename T>
std::string f(T)
{
    return "f(T)";
}

template <typename T>
std::string f(T*)
{
    return "f(T*)";
}

template <>
#if (0)
std::string f(int x = 0) //default argument not permitted on an explicit specialization of function 'f'
#else
std::string f(int x)
#endif
{
    return "f(int)";
}

template <>
std::string f(int*)
{
    return "f(int*)";
}

} //_3 --------------------------------------------------------------

namespace _4 { //variable template specialization

template <typename T>
constexpr std::size_t SZ = sizeof(T);

template <>
constexpr std::size_t SZ<void> = 0;

} //_4 --------------------------------------------------------------

} //namespace =================================================================

#include "g.hpp"

namespace g {

template <>
int func(int, int init)
{
    return init/2;
}

void test()
{
    std::cout << func(1.0, 2.0) << "\n";
    std::cout << func(1, 2) << "\n";
}

}

void test_ch_16_specialization()
{
    g::test();
}
