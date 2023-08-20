#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

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

namespace _2 {

void printArray(const QJsonArray& array, const QString& subject)
{
    qDebug() << subject;

    for (auto iter = array.begin(); iter != array.end(); iter++) {

        auto value = (*iter);
        switch (value.type()) {
        case QJsonValue::String:
            qDebug() << "\t" << value.toString();
            break;
        case QJsonValue::Double:
            qDebug() << "\t" << value.toInt();
            break;
        case QJsonValue::Bool:
            qDebug() << "\t" << value.toBool();
            break;
        default:
            qDebug() << "\t" << value;
            break;
        }
    }
}

void test(const char* filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        auto data = file.readAll();
        file.close();

        auto doc = QJsonDocument::fromJson(data);

        auto id = doc["id"].toString().toInt();
        qDebug() << "id: " << id;

        //...
    }
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_json(const char* filename)
{
#if (0)
    _1::test();
#endif

    _2::test(filename);
}
