#include <QAtomicInteger>

// Memory Order
//  Relaxed
//      the compiler and processor to freely reorder memory accesses
//  Acquire
//      memory access following the atomic operation may not be re-ordered before the atomic operation
//  Release
//      memory access before the atomic operation may not be re-ordered after the atomic operation
//  Ordered
//      the same Acquire and Release sematics combined

namespace { //=================================================================

namespace _1 {

struct My {
    QAtomicInt value;
};

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_atomic()
{

}
