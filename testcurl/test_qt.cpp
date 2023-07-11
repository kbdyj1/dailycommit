#include "TestCurl.h"

namespace { //=================================================================

} //===========================================================================

void test_qt(QObject* parent)
{
    auto* test = new TestCurl(parent);

#if (1)
    test->get();
#else
    test->post();
#endif
}
