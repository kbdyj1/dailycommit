#include <QStringView>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void func0(QStringView s)
{
    qDebug() << "func0(QStringView) " << s;
}

void func1(const QStringView& s)
{
    qDebug() << "func1(const QStringView&) " << s;
}

void func2(QString s)
{
    qDebug() << "func2(QString)" << s;
}

void test()
{
    func0(u"Hello, Qt5");
    func1(u"Hello, Qt6");

    func2("Hello, Qt6");

    QString s{};

    qDebug() << s.data();

    QStringView v{};

    qDebug() << v.data();
}

} //_1 --------------------------------------------------------------

namespace _2 {

void f(QStringView) { qDebug() << "f(QStringView)"; }
void f(QLatin1String) { qDebug() << "f(QLatin1String)"; }
void f(QChar c) { qDebug() << "f(QChar)"; return f(QStringView{&c, 1}); }
void f(const QString& s) { qDebug() << "f(const QString&)"; return f(QStringView{s}); }

void test()
{
    f("Hello, Qt6");
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_stringview()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
