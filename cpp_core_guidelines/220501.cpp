#include <iostream>

namespace { //=================================================================

// Enum.1 Prefer enumerations over macros

enum class WebColor {
    Red = 0xff0000,
    Green = 0x00ff00,
    Blue = 0x0000ff
};

enum class ProductInfo {
    Red = 0,
    Green,
    Blue
};

// Enum.2 use enumerations to represent sets of related named constants

// Enum.3 Prefer class enums over "plain" enums

// Enum.4 Define operations on enumerations for safe and simple use

enum Day {
    Mon = 0,
    Tue,
    Wed,
    Thu,
    Fri,
    Sat,
    Sun
};
Day& operator++(Day& d)
{
    return d = (d == Day::Sun) ? Day::Mon : static_cast<Day>(static_cast<int>(d)+1);
}

// Enum.5 Don't use ALL_CAPS for enumerators

// Enum.6 Avoid unnamed enumerations

#if (0)
enum {
    Red = 0xFF0000,
    Scale = 4,
    IsSigned = 1
};
#else
constexpr int Red = 0xFF0000;
constexpr short Scale = 4;
constexpr bool IsSigned = true;
#endif

// Enum.7 Specify the underlying type of an enumeration only when necessary

enum Flags : char; // forward declaration
void f(Flags);

enum class Direction : char {
    N, S, E, W,
    NE, NW, SE, SW
};

// Enum.8 Specify enumerator values only when necessary

} // namespace ================================================================

void test_enum_1()
{
#if (0)
    int webby = Blue;
#else
    // use an enum class to avoid name clashes
    WebColor webby = WebColor::Blue;
#endif
}
