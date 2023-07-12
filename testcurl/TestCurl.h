#ifndef TESTCURL_H
#define TESTCURL_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>

class TestCurl : public QObject
{
    Q_OBJECT

public:
    TestCurl() = delete;
    TestCurl(QObject* parent);
    virtual ~TestCurl();

    void get();
    void post();

protected slots:
    void onPostFinished();
    void onGetFinished();
    void onError(QNetworkReply::NetworkError);

private:
    QNetworkAccessManager* network;
};

TestCurl::TestCurl(QObject* parent)
    : QObject(parent)
    , network(new QNetworkAccessManager(this))
{}
TestCurl::~TestCurl()
{}

void TestCurl::post()
{
    const QUrl url("https://www.example.com");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj["title"] = "this is title";
    obj["scheduledStartTime"] = "2023-07-11";
    QJsonObject status;
    status["privacyStatus"] = "private";
    obj["status"] = status;
    QJsonObject snippet;
    snippet["snippet"] = obj;

    QJsonDocument doc(snippet);
    QByteArray data = doc.toJson();

    qDebug() << data;

    auto config = QSslConfiguration{};
    config.setProtocol(QSsl::TlsV1_2);
    req.setSslConfiguration(config);

    QNetworkReply* reply = network->post(req, data);
    connect(reply, SIGNAL(finished()), this, SLOT(onPostFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
}

void TestCurl::onPostFinished()
{
    auto* reply = dynamic_cast<QNetworkReply*>(sender());
    if (reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
        } else {
            auto bytes = reply->readAll();
            QFile file("post.html");
            if (file.open(QFile::WriteOnly)) {
                file.write(bytes);
            }
        }
    }
}

void TestCurl::onError(QNetworkReply::NetworkError error)
{
    qDebug() << "onError: " << error;
}

void TestCurl::get()
{
    QNetworkRequest req(QUrl("https://google.com"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = network->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(onGetFinished()));
}

void TestCurl::onGetFinished()
{
    auto* reply = dynamic_cast<QNetworkReply*>(sender());
    if (reply) {
        auto bytes = reply->readAll();
        QFile file("get.html");
        if (file.open(QFile::WriteOnly)) {
            file.write(bytes);
        }
    }
}

#include "moc_TestCurl.cpp"

#endif // TESTCURL_H
