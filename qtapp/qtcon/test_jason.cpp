#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    char stream[] = { 0x20, 0x30, 0x40, 0x50 };
    QByteArray ba = QByteArray::fromRawData(stream, 4);
    QString str = QString::fromStdString(ba.toBase64().toStdString());
    QJsonObject o;
    QJsonValue value(str);
    o.insert("stream", value);

    qDebug() << o;
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================


void test_jason()
{
    _1::test();
}
