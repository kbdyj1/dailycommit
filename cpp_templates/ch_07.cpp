//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.07 Pass by value or pass by reference
//
//  22/09/25
//=============================================================================

#include <iostream>
#include <string>
#include <functional>

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

namespace passByRValue {

template <typename T>
void pass(T&& arg) {
    //T x;
    std::cout << arg << ", [" << typeid(T).name() << "]\n";
}

void test()
{
    pass(62);

    const auto c = "Hi"s;
    pass(c);
    pass("Hi");
    int arr[4];
    pass(arr);
}

} //passByRValue ----------------------------------------------------

void printString(const std::string& s)
{
    std::cout << "printString(const std::string&)\n";
}

template <typename T>
void printT(T arg)
{
    printString(arg);
}

void f(int& n0, int& n1, const int& n2)
{
    std::cout << "f(" << n0 << ", " << n1 << ", " << n2 << ")\n";
    ++n0;
    ++n1;
}

void test_cref()
{
    auto s = "Hello, Qt"s;

    printT(s);
    printT(std::cref(s));

    int n0 = 1;
    int n1 = 2;
    int n2 = 3;

    auto bound_f = std::function<void()>{ std::bind(f, n0, std::ref(n1), std::cref(n2))};
    n0 = 10;
    n1 = 11;
    n2 = 12;

    std::cout << "before: " << n0 << ", " << n1 << ", " << n2 << "\n";

    bound_f();

    std::cout << "after: " << n0 << ", " << n1 << ", " << n2 << "\n";
}

template <typename T>
int compareArray(const T* p0, size_t S0, const T* p1, size_t S1) {
    for (auto i=0; i<S0 || i<S1; i++) {
        if (p0[i] < p1[i])
            return 1;
        if (p0[i] > p1[i])
            return -1;
    }
    return S0 - S1;
}

template <typename T, size_t S0, size_t S1>
void foo(T(&arg0)[S0], T(&arg1)[S1])
{
    T* p0 = arg0;
    T* p1 = arg1;
    auto ret = compareArray(p0, S0, p1, S1);
    std::cout << "compareArray(): " << ret << "\n";
}

void test_array()
{
    int array0[] = { 1, 2, 3, 4 };
    int array1[] = { 1, 2, 3 };
    int array2[] = { 1, 3, 2 };

    foo(array0, array1);
    foo(array1, array0);
    foo(array1, array2);
}

template <typename T>
#if (1)
typename std::remove_reference<T>::type bar(T p)
#else
auto bar(T p) //c++14
#endif
{
    std::cout << "bar(" << typeid(T).name() << ")\n";
    return T{};
}

void test_return()
{
//    int i = 0;
//    bar<int&>(i);
}

} //namespace =================================================================

void test_ch_07()
{
#if (0) //done
    passByValue::test();
    passByRef::test();
    passByRef::test_out();
    passByRValue::test();
    test_cref();
#endif

    test_array();
}
