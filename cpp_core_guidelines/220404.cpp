#include <iostream>
#include <memory.h>
#include <iterator>

#define Ensure(statement)

namespace { //=================================================================

// I6. Prefer Expects() for expressing preconditions

// I7. State postconditions

int areaBad(int h, int w) {
    return h * w;
}

int areaGood(int h, int w) {
    auto res = h * w;
    Ensure(res > 0);
    return res;
}

const int MAX_BUF = 32;

void fBad()
{
    char buf[MAX_BUF];

    memset(buf, 0, sizeof(buf));
}

void fGood()
{
    char buf[MAX_BUF];

    memset(buf, 0, sizeof(buf));

    Ensure(buf[0] == 0);
}

// I8. Prefer Ensures() for expressing postconditions

// I9. If an interface is a template, document its parameters using concepts

template <typename Iter, typename Val>
//requires InputIterator<Iter> && EqualityComparable<ValueType<Iter>, Val>
Iter find(Iter first, Iter last, Val val)
{
}

// I10. Use exceptions to signal a failure to perform a required task

} // namespace ================================================================

void test_i_7()
{
    auto w = 10;
    auto h = 20;

    auto res = areaGood(h, w);

    std::cout << "areaGood(" << h << ", " << w << ") : " << res << std::endl;

    fGood();
}
