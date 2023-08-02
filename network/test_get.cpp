#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Test : public QObject
{
public:
    Test(const QString& url, QObject* parent = nullptr) : QObject(parent)
    {
        auto* req = new QNetworkRequest(url);
        auto* reply = nam.get(*req);
        connect(reply, &QNetworkReply::finished, this, &Test::onFinished);
    }

protected Q_SLOTS:
    void onFinished()
    {
        auto* reply = dynamic_cast<QNetworkReply*>(sender());
        {
            auto responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            auto url = reply->url();
            qDebug() << url << " -> " << responseCode;
        }
    }
private:
    QNetworkAccessManager nam;
};

void test_get(QObject* parent)
{
    new Test("https://doc.qt.io/qt-6/qnetworkaccessmanager.html", parent);
}
