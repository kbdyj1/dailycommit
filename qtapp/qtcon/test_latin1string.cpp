#include <QLatin1String>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    auto str = QString{"Qt"};
    auto isSame = str == "Qt";
    qDebug() << "str == \"Qt\" ? " << isSame;

    isSame = str == QString{"Qt"};
    qDebug() << "str == QString{\"Qt\"} ? " << isSame;

    isSame = str == QLatin1String{"Qt"};
    qDebug() << "str == QLatin1String{\"Qt\"} ? " << isSame;
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    auto str = QLatin1String{"Qt"};
    qDebug() << QString{}.asprintf("%p", str.data());

    auto str2 = QLatin1String{"Qt"};
    qDebug() << QString{}.asprintf("%p", str2.data());

    auto str3 = QLatin1String{"abcdefghijklmnopqrstuvwxyz"};
    str3.chop(4);
    qDebug() << "str3.chop(4): " << str3;

    auto str4 = str3.chopped(4);
    qDebug() << "str3.chopped(4): " << str4;

    str3.chop(str3.size());
    qDebug() << "str3.isEmpty() ? " << str3.isEmpty();
    qDebug() << "str3.isNull() ? " << str3.isNull();
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_latin1string()
{
#if (0)
    _1::test();
#endif

    _2::test();
}
