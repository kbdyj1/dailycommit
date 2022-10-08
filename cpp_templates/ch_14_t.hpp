#ifndef __CH_14_T_HPP
#define __CH_14_T_HPP

#include <iostream>

template <typename T>
void externalFunc()
{
    std::cout << "ch_14_t.hpp : externalFunc<" << typeid(T).name() << ">\n";
}

extern template void externalFunc<int>();

#endif
