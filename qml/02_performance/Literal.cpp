#include <QByteArray>
#include <QDebug>

void test_literal()
{
    QByteArray b0 = QByteArrayLiteral("Hello, Qt");
    QByteArray b1("Hello, Qt");
    QByteArray b2(16, 'x');

    static char rawData[] = { 0x1, 0x2, 0x3, 0x4 };
    QByteArray b3 = QByteArray::fromRawData(rawData, 4);
    QByteArray b4;

    b4.resize(4);
    for (auto i=0; i<4; i++) {
#if (1)
        //deep copy
        b4[i] = b4.at(i) + 'a';

        qDebug() << b4;
#else
        b4.at(i);
#endif

    }
}
