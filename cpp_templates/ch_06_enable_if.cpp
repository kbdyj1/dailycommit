#include <iostream>

namespace { //=================================================================

template <typename T>
using EnableIfSizeGreater4 = std::enable_if_t<(sizeof(T) > 4)>;

template <typename T, typename = EnableIfSizeGreater4<T>>
void f()
{
    std::cout << "f() T=" << typeid(T).name() << "\n";
}

} //namespace =================================================================

void test_ch_06_enable_if()
{
    f<double>();
    f<std::string>();
}
