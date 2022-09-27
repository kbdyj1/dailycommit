#include <iostream>
#include <vector>

namespace { //=================================================================

template <typename T, unsigned N>
std::size_t len(T(&)[N])
{
    return N;
}

//#define USE_TRAILING_RETURN_TYPE

template <typename T>
#if !defined(USE_TRAILING_RETURN_TYPE)
typename T::size_type len(const T& t)
#else
auto len(const T& t) -> decltype( (void)(t.size()), T::size_type())
#endif
{
    return t.size();
}

std::size_t len(...)
{
    return 0;
}

void test_array_size()
{
    int a[10];

    std::cout << "len(a): " << len(a) << "\n";
    std::cout << R"(len("Hello, Qt"): )" << len("Hello, Qt") << "\n";

#if !defined(USE_TRAILING_RETURN_TYPE)
    auto v = std::vector<int>{ 1, 2, 3, 4 };
    std::cout << "len(v): " << len(v) << "\n";
#endif

    int* p = nullptr;
    std::cout << "len(p): " << len(p) << "\n";

#if (0)
    // error: ‘const class std::allocator<int>’ has no member named ‘size’
    // 15 |     return t.size();
    //    |            ~~^~~~
    auto alloc = std::allocator<int>{};
    std::cout << "len(alloc): " << len(alloc) << "\n";
#endif
}

} //namespace =================================================================

void test_ch_08_sfinae()
{
    test_array_size();
}
