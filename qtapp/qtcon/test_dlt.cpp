#include <dlt/dlt.h>

DLT_DECLARE_CONTEXT(ctx);

namespace { //=================================================================

namespace _1 {

void test()
{
    DLT_LOG(ctx, DLT_LOG_ERROR, DLT_CSTRING("_1::test()"));
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_dlt()
{
    DLT_REGISTER_APP("QCON", "qtcon");
    DLT_REGISTER_CONTEXT(ctx, "TEST", "Text context for dlt");

    _1::test();

    DLT_UNREGISTER_CONTEXT(ctx);
    DLT_UNREGISTER_APP();
}
