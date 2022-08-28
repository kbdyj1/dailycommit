#ifndef UTIL_H
#define UTIL_H

#include <iterator>
#include <iostream>

template <typename C>
void print(const C& c)
{
    std::copy(std::begin(c), std::end(c), std::ostream_iterator<typename C::value_type>(std::cout, " "));
    std::cout << "\n";
}

#define STRINGFY2(x)    #x
#define STRINGFY(x)     STRINGFY2(x)
#define CONNECT2(x, y)  x##y
#define CONNECT(x, y)   CONNECT2(x, y)

#define PRINT_FUNC(x)   std::cout << STRINGFY(x) << ": " << x << "\n"

#endif // UTIL_H
