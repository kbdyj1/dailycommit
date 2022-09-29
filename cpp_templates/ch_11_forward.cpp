#include <iostream>

namespace { //=================================================================

template <typename T>
T get(T t)
{
    return t*2;
}

template <typename T>
void set(T&& t)
{
    (void)t;
}

template <typename T>
void f(T t)
{
    auto&& val = get(t);

    set(std::forward<decltype(val)>(val));
}

} //namespace =================================================================

void test_ch_11_forward()
{

}
