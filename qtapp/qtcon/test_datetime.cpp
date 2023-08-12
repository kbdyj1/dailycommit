#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>

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

namespace _3 {

void test()
{
    auto et = QElapsedTimer{};
    et.start();

    qDebug() << "msecsSinceReference: " << et.msecsSinceReference() << "msecs";

    auto msecs = et.msecsSinceReference();

    auto sec = msecs / 1000;
    auto min = sec / 60;
    auto hour = min / 60;
    auto ss = sec % 60;
    auto mm = min % 60;
    auto str = QString{"PT%1H%2M%3S"}.arg(hour).arg(mm).arg(ss);

    qDebug() << hour << ":" << (min % 60) << ":" << (sec % 60) << " -> " << str;

    auto current = QDateTime::currentDateTime();
    auto past = current.addSecs(-sec);

    qDebug() << "current : " << current;
    qDebug() << "boot time: " << past;
}

} //_3 --------------------------------------------------------------

} //===========================================================================

void test_datetime()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
