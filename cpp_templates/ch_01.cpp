//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.01 template function
//
//  22/09/19
//=============================================================================

#include <iostream>
#include <type_traits>

namespace { //=================================================================

template <typename T>
T foo(T*)
{
    std::cout << typeid(T).name() << "\n";
}

#if (0)
template <typename T>
void bar(T t)
{
    undeclared();
    undeclared(t);

    static_assert(4 <= sizeof(int), "int too small");
}
#endif

template <typename T>
T max(T a, T b)
{
    return (a < b) ? b : a;
}

void test_type_deduction()
{
    const int c = 62;
    int i = 40;

    auto res = max(i, c);

    int& r = i;

    res = max(i, r);
}

#define DEFINE_DEFAULT_TYPE

#ifdef  DEFINE_DEFAULT_TYPE
template <typename T = std::string>
#else
template <typename T>
#endif
void f(T = "")
{
    std::cout << "f(" << typeid(T).name() << ")\n";
}

void test_type_deduction_default_argument()
{
    f(1);

#if defined(DEFINE_DEFAULT_TYPE)
    f(); //f(NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE)
#else
    f(); //couldn’t deduce template parameter ‘T’
#endif
}

void test_void_type_deduction()
{
    void* p = nullptr;
    foo(p);
}

template <typename Ret, typename T0, typename T1>
Ret func(T0 a, T1 b)
{
    return a < b ? b : a;
}

void test_multiple_type_deduction()
{
    auto ret = func<double>(1, 2.5);

    std::cout << "func<double>(1, 2.5): " << ret << "\n";
}

#if 201103L < __cplusplus //C++14

template <typename T0, typename T1>
auto max2(T0 a, T1 b) -> typename std::decay<decltype(true?a:b)>::type
{
    return b < a ? a : b;
}

void test_return_type_auto_deduction()
{
    auto ret = max2(1, 2.4f);
    std::cout << "max2(1, 2.4): " << ret << ", type: " << typeid(ret).name() << "\n";

    ret = max2(10, 3.8f);
    std::cout << "max2(10, 3.8f): " << ret << ", type: " << typeid(ret).name() << "\n";

    int i = 2;
    float j = 3.4;
    int& r0 = i;
    float& r1 = j;

    ret = max2(r0, r1);
    std::cout << "max2(r0, r1): " << ret << ", type: " << typeid(ret).name() << "\n";
}
#endif

template <typename T0, typename T1>
std::common_type_t<T0, T1> max3(T0 a, T1 b)
{
    return a < b ? b : a;
}

void test_common_type()
{
    auto ret = max3(3, 4.0f);
    std::cout << "max3(3, 4.0f): " << ret << ", type: " << typeid(ret).name() << "\n";
}

template <typename T0, typename T1, typename Ret = std::decay<decltype(true ? T0() : T1())>>
Ret max4(T0 a, T1 b)
{
    return b < a ? a : b;
}

} //namespace =================================================================

void test_ch_01_max();

void test_ch_01()
{
#if (0) // done
    test_void_type_deduction();
    test_type_deduction_default_argument();
    test_multiple_type_deduction();
    test_common_type();
#endif

#if 201103L < __cplusplus
    //test_return_type_auto_deduction();
#endif

    test_ch_01_max();
}
