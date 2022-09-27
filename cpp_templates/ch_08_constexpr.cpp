#include <iostream>
#include <string>

using namespace std::string_literals;

namespace { //=================================================================

namespace cxx17 {

template <typename T, typename... Args>
void print(const T& arg, const Args&... args)
{
    if constexpr (std::is_same_v<T, std::string>)
        std::cout << arg << " ";
    else
        std::cout << arg;

    if constexpr(0 < sizeof...(args)) {
        print(args...);
    }
}

void test_print()
{
    auto s = "Hello"s;

    print(s, "Qt"s, 6, '.', 2.0);
}

void test()
{
    test_print();
}

} //cxx17------------------------------------------------------------

template <typename T>
void foo(T t)
{
    if constexpr(std::is_integral_v<T>) {
        if (t > 0)
            foo(t-1);
    } else {
        dummy(t);
//        dummy();
//        static_assert(false, "no integral");
        static_assert(!std::is_integral_v<T>, "no integral");
    }
}

} //namespace =================================================================

void test_ch_08_constexpr()
{
    cxx17::test();
}
