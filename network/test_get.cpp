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
            auto reasonPhrase = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
            auto redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (redirect.isValid()) {
                qDebug() << "Redirect -> " << redirect.toUrl();
            }
            auto sourceFromCache = reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
            if (sourceFromCache.isValid()) {
                qDebug() << "Cached? -> " << sourceFromCache.toBool();
            }

            auto url = reply->url();
            qDebug() << url << " -> " << responseCode << "(" << reasonPhrase << ")\n";

            printHeaderInfo(reply);
        }
    }

private:
    void printHeaderInfo(QNetworkReply* reply)
    {
        auto headers = reply->rawHeaderList();
        for (const auto& header : headers) {
            qDebug() << header << ": " << reply->rawHeader(header);
        }
    }
    QNetworkAccessManager nam;
};

void test_get(QObject* parent)
{
    new Test("https://doc.qt.io/qt-6/qnetworkaccessmanager.html", parent);
}
