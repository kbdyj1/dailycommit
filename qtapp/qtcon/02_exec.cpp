#include <QObject>
#include "TestExec.h"
#include <QTimer>

namespace {  //================================================================

namespace _1 {

void test(QObject* parent)
{
    auto* instance  = new TestExec(parent);
    instance->start();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_exec(QObject* parent)
{
    _1::test(parent);
}
