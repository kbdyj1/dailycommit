#include <iostream>

namespace { //=================================================================

// Substitution Failure Is Not An Error
// 1. exactly matching
// 2. template
// 3. variable argument

template <typename T>
void foo(T t)
{
    std::cout << "foo(T)" << std::endl;
}

void foo(int n)
{
    std::cout << "foo(int)" << std::endl;
}

void foo(...)
{
    std::cout << "foo(...)" << std::endl;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, int>::type
bar(T a)
{
    std::cout << "bar(T)" << std::endl;

    return a * 2;
}

void bar(...)
{
    std::cout << "bar(...)" << std::endl;
}

void PRINT(const std::string& msg)
{
    std::cout << msg << std::endl;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value, int>::type
func(T a)
{
    PRINT("func(T) #1");
    return a;
}

template <typename T>
void func(T a, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr)
{
    PRINT("func(T, T*)");
}

template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
void func(T a)
{
    PRINT("func(T) #2");
}

void func(...)
{
    PRINT("func(...)");
}

#if (0)
template <typename T>
void printv(const T& v)
{
    if constexpr(std::is_pointer<T>::value)
        std::cout << v << ":" << *v << std::endl;
    else
        std::cout << v << std::endl;
}
#else
template <typename T>
void printvImpl(const T& v, std::true_type)
{
    std::cout << v << " : " << *v << std::endl;
}
template <typename T>
void printvImpl(const T& v, std::false_type)
{
    std::cout << v << std::endl;
}

template <typename T>
void printv(const T& v)
{
    printvImpl(v, std::is_pointer<T>());
}

#endif

} // namespace ================================================================

void test_sfinae()
{
    std::cout << std::endl;

    foo(3);
    foo("Hello, Qt6");

    bar(2.4);
    auto res = bar(3);
    std::cout << "bar(3) : " << res << std::endl;

    //func(2);
    func(2.3);

    std::string msg = "Qt, 6";
    printv(&msg);
    printv(msg);
}
