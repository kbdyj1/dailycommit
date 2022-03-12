#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFuture>

namespace { //=================================================================

void test0()
{
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QUrl url("https://api.spark.io/v1/devices/53ff70066667574852212067/reading?access_token=94f1858893b749831ee5704732a1814beea6b678");
    QNetworkRequest req(url);
    auto *reply = manager.get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        auto str = QString{ reply->readAll() };
        qDebug() << "res: " << str;
        auto jsonRes = QJsonDocument::fromJson(str.toUtf8());
        auto jsonObj = jsonRes.object();
        qDebug() << "result: " << jsonObj["result"].toInt();
    } else {
        qDebug() << "fail: " << reply->errorString();
    }
    delete reply;
}

void test1()
{
    auto manager = QNetworkAccessManager{};
    auto url = QUrl{"https://api.spark.io/v1/devices/53ff70066667574852212067/reading?access_token=94f1858893b749831ee5704732a1814beea6b678"};
    auto *reply = manager.get(QNetworkRequest{url});
    auto future = QtFuture::connect(reply, &QNetworkReply::finished)
            .then([reply]{
                return reply->readAll();
            });
}

} // namespace ================================================================

void testNetwork()
{
    //test0();
    test1();
}
