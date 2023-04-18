#include <QByteArray>
#include <QDebug>

namespace {  //================================================================

namespace _1 {

// at() faster than operator[]()
void test()
{
    QByteArray ba{"Hello, Qt"};

    for (qsizetype i=0; i<ba.size(); i++) {
        if (ba.at(i) >= 'a' && ba.at(i) <= 'f')
            qDebug() << "found character in range [a-f]: " << ba.at(i);
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    QByteArray ba0{"ca\0\r\t"};
    qDebug() << ba0.size() << " -> " << ba0.constData();

    QByteArray ba1{"ca\0\r\t", 3};
    qDebug() << ba1.size() << " -> " << ba1.constData();

    QByteArray ba2{"ca\0\r\t", 4};
    qDebug() << ba2.size() << " -> " << ba2.constData();

    const char cart[] = {'c', 'a', '\0', 'r', '\0', 't'};
    QByteArray ba3(QByteArray::fromRawData(cart, 6));
    qDebug() << ba3.size() << " -> " << ba3.constData();
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_qbytearray()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
