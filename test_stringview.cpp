#include <QStringView>
#include <QString>
#include <mutex>
#include <QDebug>

namespace internal
{

void testCTAD()
{
    // C++17 feature
    // Class Template Argument Deduction
    std::mutex m;
    std::unique_lock lock(m); // std::unique_lock<std::mutex> lock;
}

void testTockenizer()
{
    // QStringTockenizer splits strings, with zero memory allocations !!!

    const auto s0 = QString{ "QString\nQt 6.2.2\nUbuntu 20.04" };
    for (QStringView v : QStringTokenizer{s0, u'\n'})
        qDebug() << v;

    const auto s1 = std::u16string{ u"std::u16string\nQt 6.2.2\nUbuntu 20.04" };
    for (QStringView v : QStringTokenizer{s1, u'\n'})
        qDebug() << v;

    const auto s2 = QLatin1String{ "QLatin1String\nQt 6.2.2\nUbuntu 20.04" };
    for (auto v : QStringTokenizer{s2, u'\n'})
        qDebug() << v;

    // < C++17
    for (QLatin1String s : qTokenize(s2, u'\n'))
        qDebug() << s;

    auto result = s2.tokenize(u'\n');
    for (auto s : result)
        qDebug() << "s2.tokenize(): " << s;
}

}

void testStringView()
{
    internal::testTockenizer();
}
