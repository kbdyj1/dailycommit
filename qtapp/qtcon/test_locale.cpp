#include <QLocale>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    QLocale egyptian{QLocale::Arabic, QLocale::Egypt};
    auto s0 = egyptian.toString(1.57e+07, 'e');
    auto s1 = egyptian.toString(10);
    auto d = egyptian.toDouble(s0);
    auto i = egyptian.toInt(s1);

    qDebug() << s0 << "->" << d;
    qDebug() << s1 << "->" << i;
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    QLocale::setDefault(QLocale(QLocale::Hebrew, QLocale::Israel));
    QLocale hebrew;
    QString s0 = hebrew.toString(15714.3, 'e');

    bool ok;
    double d;

    QLocale::setDefault(QLocale::C);

    d = QString{"1234,56"}.toDouble(&ok);
    d = QString{"1234.56"}.toDouble(&ok);

    QLocale::setDefault(QLocale{QLocale::English, QLocale::UnitedStates});
    auto str = QString{"%1 %L2 %L3"}.arg(12345).arg(12345).arg(12345, 0, 16);

    qDebug() << str;
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    QLocale korean{"ko"};
    qDebug() << korean.language() << ", " << korean.country();
    qDebug() << korean.currencySymbol();
    qDebug() << "toCurrencyString(1000): " << korean.toCurrencyString(1000);

    QLocale swiss{"de_CH"};
    qDebug() << swiss.language() << ", " << swiss.country();
}

} //_3 --------------------------------------------------------------

} //===========================================================================


void test_locale()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
