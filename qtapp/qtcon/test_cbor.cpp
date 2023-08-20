#include <QCborValue>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

namespace {  //================================================================

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
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    auto root = QJsonObject{};
    auto avnt = QJsonObject{};
    root["avnt"] = avnt;
    avnt["platform"] = "Linux";
    avnt["country"] = "KR";
    auto coord = QJsonObject{};
    coord["lat"] = 37.0;
    coord["lon"] = 127.0;
    avnt["coord"] = coord;

    auto doc = QJsonDocument{root};
    auto bytes = doc.toJson();
    qDebug() << "json.size: " << bytes.size();
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_cbor()
{
    _1::test();
    _2::test();
}
