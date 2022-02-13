#include <QLocale>

namespace internal
{

void test_egytian()
{
    auto locale = QLocale{QLocale::Arabic, QLocale::Egypt};
    auto s1 = locale.toString(1.571429E+07, 'e');
    qDebug() << s1;

    auto s2 = locale.toString(10);
    qDebug() << s2;

    auto d = locale.toDouble(s1);
    qDebug() << d;

    auto i = locale.toInt(s2);
    qDebug() << i;
}

void test_default()
{
    auto locale = QLocale::system();
    auto language = locale.language();
    auto languageStr = locale.languageToString(language);
    auto territory = locale.territory();
    auto territoryStr = locale.territoryToString(territory);

    qDebug() << "lang:" << languageStr << ", country:" << territoryStr;
}

void test_group_separator()
{
    auto n = 12345;
    auto str = QString{"%1 %L2 %L3"}.arg(n).arg(n).arg(n, 0, 16);

    qDebug() << str;
}

}

void test_locale()
{
    //internal::test_egytian();
    //internal::test_default();
    internal::test_group_separator();
}
