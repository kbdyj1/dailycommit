#include <QLocale>
#include <QDate>

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

void print_locale_info(const QLocale &locale)
{
    auto language = locale.language();
    auto languageStr = locale.languageToString(language);
    auto territory = locale.territory();
    auto territoryStr = locale.territoryToString(territory);

    qDebug() << "lang:" << languageStr << ", country:" << territoryStr;

    qDebug() << "currency symbole:" << locale.currencySymbol();
    qDebug() << "formatted data size:" << locale.formattedDataSize(1024*1024);
    qDebug().noquote() << "quote string(standard):" << locale.quoteString("Qt6");
    qDebug().noquote() << "quote string(alternate):" << locale.quoteString("Qt6", QLocale::AlternateQuotation);
    qDebug() << "amText:" << locale.amText();
    qDebug() << "pmText:" << locale.pmText();
    qDebug() << "bcp47Name:" << locale.bcp47Name();
    qDebug() << "dateFormat:" << locale.dateFormat() << ", " << QDate::currentDate();
    qDebug() << "dayName:" << locale.dayName(1);
    qDebug() << "decimalPoint:" << locale.decimalPoint();
    qDebug() << "exponential:" << locale.exponential();
    for (auto i=1; i<12; i++) {
        qDebug() << "month(" << i << "):" << locale.monthName(i);
    }
    qDebug() << "zeroDigit:" << locale.zeroDigit();

    qDebug() << "uiLanguages.---------------------------------------------------";
    for (auto uiLanguage : locale.uiLanguages()) {
        qDebug() << uiLanguage;
    }
    qDebug() << "uiLanguages.---------------------------------------------------";
}

void test_default()
{
    qDebug() << "system locale =================================================";
    auto locale = QLocale::system();
    print_locale_info(locale);

    qDebug() << "locale{ko} ====================================================";
    locale = QLocale{"ko"};
    print_locale_info(locale);
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
    internal::test_default();
    //internal::test_group_separator();
}
