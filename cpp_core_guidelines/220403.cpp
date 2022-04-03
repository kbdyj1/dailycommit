#include <iostream>
#include <cmath>
#include <chrono>

namespace { //=================================================================

// I1. Make interface explicit

bool gRoundUp = false;

// Bad
int round(double d)
{
    return (gRoundUp) ? ceil(d) : d; // don't: invisible dependency
}

// I2. Avoid non-const global variables

struct Context
{
    bool initialized = false;
};
Context gContext;

void test_i_2()
{
    auto const &c = gContext;

    if (c.initialized) {
        std::cout << "context.initialized: " << c.initialized;
    }
}

// I3. Avoid singletons
struct A
{
    int count = 0;
};

A& getA()
{
    // the initialization of the static object does not introduce a race condition.
    static A internalA;

    internalA.count++;

    return internalA;
}

// I4. Make interfaces precisely and strongly typed

void blankBad(int timeToBlank) // millisecond ? second ?
{
    (void)timeToBlank;
}
void blankGood(std::chrono::milliseconds duration)
{
    (void)duration;
}

template <typename Rep, typename Period>
void blankAny(std::chrono::duration<Rep, Period> timeToBlank)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeToBlank);

    (void)ms;
}

// I.5 State preconditions (if any)

double sqrt(double x)
{
    //Expects(x >= 0.0);

    return 0.0;
}

int area(int h, int w)
{
#if (1)
    //Expects(h > 0 && w > 0);
#else
    if (h <= 0 || w <= 0) {
        // error
        return 0;
    }
#endif

    return h * w;
}


} // namespace ================================================================

void test_i_3()
{
    auto a = getA();

    std::cout << a.count << std::endl;
}

void test_i_4()
{
    using namespace std::chrono_literals;

    blankGood(1500ms);
    blankAny(2s);
    blankAny(2500ms);
}
