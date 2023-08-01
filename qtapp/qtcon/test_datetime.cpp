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

namespace _2 {

void test()
{
    auto dt = QDateTime::fromString("PT36H", Qt::ISODate);

    qDebug() << dt;
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_datetime()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
