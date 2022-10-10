#include <iostream>
#include <vector>

namespace { //=================================================================

namespace _1 {

template <typename T, unsigned N>
T* cBegin(T(&array)[N])
{
    return array;
}
template <typename Container>
typename Container::iterator cBegin(Container& c)
{
    return c.begin();
}

void test()
{
    auto v = std::vector<int>{1, 2, 3, 4, 5};
    int array[16];

    auto b0 = cBegin(v);
    auto b1 = cBegin(array);
}

} //_1 --------------------------------------------------------------

namespace _2 {

template <typename T>
class Array {
public:
    using iterator = T*;
};

template <typename T>
void f(typename Array<T>::iterator b, typename Array<T>::iterator e)
{}

template <typename T>
void f(T*, T*);

void test()
{
#if (0)
    f<int&>(0, 0);  //'iterator' declared as a pointer to a reference of type 'int &'
#endif
}

} //_2 --------------------------------------------------------------

namespace _3 {

template <typename T>
auto f(T t) {
    return t->m;
}

int f(...);

template <typename T>
auto g(T t) -> decltype(f(t));

void test()
{
#if (0)
    g(42);  //member reference type 'int' is not a pointer
#endif
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_15_sfinae()
{

}
