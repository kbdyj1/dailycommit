//=============================================================================
//  A tour of C++
//  Ch.06  Concept & Generic Programming
//
//  22/09/15
//=============================================================================

#include <iostream>
#include <type_traits>

namespace { //=================================================================

template <typename C, typename V>
#if 201703L < __cplusplus
requires std::is_arithmetic_v<V>
#endif
V sum(const C& c, V value) {
    for (const auto& i : c) {
        value += i;
    }
    return value;
}

#if 201703L < __cplusplus
template <typename T>
concept equality_compare = requires (T a, T b) {
    { a == b } -> bool;
    { a != b } -> bool;
    { b == a } -> bool;
    { b != a } -> bool;
};
#endif

} //namespace =================================================================

void test_ch_07()
{

}
