#include <iostream>
#include <algorithm>

namespace { //=================================================================

// Per.1 Don't optimize without reason

// Per.2 Don't optimize prematurely

// Per.3 Don't optimize something that's not preformance critical

// Per.4 Don't assume that complicated code is necessarily faster than simple code

// Per.5 Don't assume that low-level code is necessarily faster than high level code

// Per.6 Don't make claims about performance without measurements

// Per.7 Design to enable optimization

void csort(void* base, std::size_t num, std::size_t size, int (*)(const void*, const void*))
{
    // ...
}

int compareDouble(const void* v0, const void* v1)
{
    const double d0 = *static_cast<const double*>(v0);
    const double d1 = *static_cast<const double*>(v1);
    return d0 - d1;
}

void use_csort()
{
    double data[100];

    csort((void*)&data[0], 100, sizeof(double), compareDouble);
}

void use_cpp_sort()
{
    double data[100];

    std::sort(data, data+100);
}

} // namespace ================================================================
