//=============================================================================
//  C++ templates. the complete guide. 2nd
//  Ch.10 Basic template terminology
//
//  22/09/28
//=============================================================================

#include <iostream>
#include <string>

namespace { //=================================================================

// primary template
template <typename T0, typename T1>
class My {
};

// explicit specialization
template<>
class My<int, std::string> {
};

template <typename T>
class My<T, T> {
};

// partial specialization
template <typename T>
class My<bool, T> {
};

namespace incomplete {

class A;
const A* pa;
extern A elems[10];
extern int arr[];

} //incomplete

namespace complete {

class A
{};
int arr[10];

} //complete

} //namespace =================================================================

class ODR
{
public:
    ODR() { std::cout << "ch10. ODR{}\n"; }
};

void test_ch_10()
{
    auto odr = ODR{};
}
