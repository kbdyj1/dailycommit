#include <QDateTime>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    auto current = QDateTime::currentDateTime();
    auto offset = current.offsetFromUtc();
    current.setOffsetFromUtc(offset);

    qDebug() << current.toString(Qt::ISODate);  //"2023-08-01T01:29:13+09:00"
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_datetime()
{
    _1::test();
}
