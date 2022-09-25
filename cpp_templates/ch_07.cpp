//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.07 Pass by value or pass by reference
//
//  22/09/25
//=============================================================================

#include <iostream>
#include <string>

using namespace std::string_literals;

namespace { //=================================================================

std::string retString() {
    return "ret"s;
}
const std::string retConstString() {
    return "ret"s;
}

namespace passByValue {
template <typename T>
void print(T arg)
{
    std::cout << arg << ", [" << typeid(T).name() << "]\n";
}

void test()
{
    auto s = "Hello, Qt"s;

    //copy
    print(s);

    //move semantics
    print(std::string{"Hi"});
    print(retString());
    print(std::move(s));
    print("Hi");
}

} //passByValue -----------------------------------------------------

namespace passByRef {

template <typename T>
void print(const T& arg)
{
    std::cout << arg << ", [" << typeid(T).name() << "]\n";
}

void test()
{
    auto s = "Hello, Qt"s;

    //copy
    print(s);

    //move semantics
    print(std::string{"Hi"});
    print(retString());
    print(std::move(s));
    print("Hi");
}

//#define PROTECT_NON_CONST

template <typename T>
void out(T& arg)
{
#if defined(PROTECT_NON_CONST)
    static_assert (!std::is_const<T>::value, "need const");
#endif

    if (std::is_array_v<T>) {
        std::cout << "array: " << std::extent_v<T> << " elems.\n";
    } else {
        std::cout << "not an array\n";
    }
}

void test_out()
{
    const auto c = std::string{"hi"};

#if !defined(PROTECT_NON_CONST)
    out(c);
    out(retConstString());
    out(std::move(c));
    out("hi");
#endif
}

} //passByRef -------------------------------------------------------

} //namespace =================================================================

void test_ch_07()
{
#if (0) //done
    passByValue::test();
    passByRef::test();
#endif

    passByRef::test_out();
}
