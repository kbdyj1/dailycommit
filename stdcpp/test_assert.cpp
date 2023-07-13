#include <iostream>

#define ENABLE_ASSERT

#if defined(ENABLE_ASSERT)
#   define debugBreak()    __asm__("int3")
#   define  ASSERT(exp)\
                if (exp) {\
                } else {\
                    std::cerr << "ASSERT: " << #exp << " " __FILE__ << "( " << __LINE__ << " )\n";\
                    debugBreak();\
                }
#else
#   define  ASSERT(exp)
#endif

#define STATIC_ASSERT0(a, b)    a ## b
#define STATIC_ASSERT1(a, b)    STATIC_ASSERT0(a, b)

#if __cplusplus < 201103L
#define STATIC_ASSERT(exp)\
    enum {\
        STATIC_ASSERT1(g_assert_fail, __LINE__)\
            = 1 / (int)(!!(exp))\
    }
#else
template<bool> class StaticAssert;
template<> class StaticAssert<true>{};

#define STATIC_ASSERT(exp)\
    enum {\
        STATIC_ASSERT1(g_assert_fail, __LINE__)\
            = 1 / sizeof(StaticAssert<!!(exp)>)\
    }
#endif

STATIC_ASSERT(sizeof(int) == 4);

namespace { //=================================================================

namespace _1 {

void test()
{
    int result = 1;

    ASSERT(result == 0);
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_assert()
{
    _1::test();
}
