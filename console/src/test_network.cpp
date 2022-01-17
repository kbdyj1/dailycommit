#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

namespace internal
{

void testNetworkAccessManager()
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

} // internal

void testNetwork()
{
    internal::testNetworkAccessManager();
}
