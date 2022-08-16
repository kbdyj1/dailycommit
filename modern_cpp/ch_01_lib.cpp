#include <iostream>

namespace { //=================================================================

#if (1)
#   define INLINE  inline
#else
#   define INLINE
#endif

#define LIB_VERSION 1

struct Foo { int a; };

namespace lib {

#if (LIB_VERSION == 1)
INLINE namespace v1 {

    template <typename T>
    int test(T value) {
        return 1;
    }

} // v1 -------------------------------------------------------------

using namespace v1;
#endif

#if (LIB_VERSION == 2)
INLINE namespace v2 {

    template <typename T>
    int test(T value) {
        return 2;
    }

} // v2 -------------------------------------------------------------
using namespace v2;
#endif

template <>
int test(Foo value) {
    return value.a;
}

void test()
{
    std::cout << "lib::test(0): " << lib::test(0) << "\n";
    auto foo = Foo{6};
    std::cout << "lib::test(Foo{6}): " << lib::test(foo) << "\n";
}

} // lib ------------------------------------------------------------

} //namespace =================================================================

void test_ch_01_lib()
{
    lib::test();
}
