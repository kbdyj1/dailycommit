//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.05 Foundation
//
//  22/09/22
//=============================================================================

#include <vector>
#include <iostream>

namespace { //=================================================================

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


} //namespace =================================================================

void test_ch_05()
{
#if (0) // done
    test_print_container();
#endif

    test_builtin_type_initialization();
}
