//=============================================================================
//  Regular Type
//      User defined types and behaves like a built-in type.
//      Behaves like an int - support assignment, equality, and order
//
//  regular type should support the following operations
//      default constructor     T a;
//      copy constructor        T a = b;
//      destructor              ~T(a);
//      assignment              a = b;
//      equality                a == b
//      inequality              a != b
//      ordering                a < b
//
//  std::semiregular (c++20)
//      X()
//      X(const X&)
//      X& operator=(const X&)
//      X(X&&)
//      X& operator=(X&&)
//      ~X()
//      swap(X&, X&)
//
//  std::regular (c++20)
//      operator==(const X&, const X&)
//      operator!=(const X&, const X&)
//
//  template<class T>
//  concept reqular = semiregular<T> && equality_comparable<T>;
//=============================================================================

#include <iostream>
#include <functional>
#include <type_traits>
#include <vector>
#include <list>

namespace { //=================================================================

namespace _1 {

void test()
{
    std::cout << "std::is_object_v<int&>: " << std::is_object_v<int&> << '\n';
    std::cout << "std::is_object_v<std::reference_wrapper<int>>: " << std::is_object_v<std::reference_wrapper<int>> << '\n';

    auto l = std::list<int>{ 1, 2, 3, 4, 5 };
    auto v = std::vector<std::reference_wrapper<int>>{l.begin(), l.end()};

    for (auto& r : v) {
        r *= r;
    }

    for (auto i : l) {
        std::cout << i << " ";
    }
    std::cout << "\n\n";
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_regular_types()
{
    std::cout << std::boolalpha << '\n';

    _1::test();
}
