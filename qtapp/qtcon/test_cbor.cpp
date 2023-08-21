#include <QCborValue>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

namespace {  //================================================================

void saveByteArray(const QString& filename, const QByteArray& bytes)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        file.write(bytes);
    }
}

namespace _1 {

void test()
{
    auto root = QCborValue{};
    auto avnt = QCborValue{};
    avnt["platform"] = "Linux";
    avnt["country"] = "KR";
    auto coord = QCborValue{};
    coord["lat"] = 37.0;
    coord["lon"] = 127.0;
    avnt["coord"] = coord;
    root["avnt"] = avnt;

    auto bytes = root.toCbor();
    qDebug() << "cbor.size: " << bytes.size();
    qDebug() << bytes;

    saveByteArray("test.cbor", bytes);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    auto root = QJsonObject{};
    {
        auto avnt = QJsonObject{};
        {
            avnt["platform"] = "Linux";
            avnt["country"] = "KR";
        }
        auto coord = QJsonObject{};
        {
            coord["lat"] = 37.0;
            coord["lon"] = 127.0;
            avnt["coord"] = coord;
        }
        root["avnt"] = avnt;
    }

    auto doc = QJsonDocument{root};
    auto bytes = doc.toJson();
    qDebug() << "json.size: " << bytes.size();

    saveByteArray("test.json", bytes);
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_cbor()
{
    _1::test(); //test.cbor (66)
    _2::test(); //test.json (153)
}
