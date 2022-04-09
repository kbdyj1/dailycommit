#include <iostream>

namespace { //=================================================================

// F60. Prefer T* over T& when "no argument" is a valid option

// F42. Return a T* to indicate a position (only)

// F43. Never (directly or indirectly) return a pointer or a reference to a local object

// F44. Return a T& when copy is undesirable and "returning no object" isn't needed

// F45. Don't return a T&&

template <typename Func>
auto&& wrapperBad(Func f)
{
    // logCall(typeid(f));
    return f();
}

template <typename Func>
auto wrapperGood(Func f)
{
    // logCall(typeid(f));
    return f();
}

// F46. int is the return type for main()

// F47. Return T& from assignment operators

// F48. Don't return std::move(local)

struct S
{};

S fBad()
{
    S res;
    return std::move(res);
}

S fGood()
{
    S res;
    return res;
}

} // namespace ================================================================
