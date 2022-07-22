#ifndef ESTL_UTIL_H
#define ESTL_UTIL_H

#include <iostream>

template <typename C>
void print_elements(const C& c, const std::string& desc = std::string{})
{
    std::cout << desc;
    for (typename C::const_iterator iter=c.begin(); iter!=c.end(); iter++) {
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

#endif // ESTL_UTIL_H
