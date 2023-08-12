#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

void printRequestAttribute(QNetworkRequest* req)
{
    qDebug() << "peerVerifyName: " << req->peerVerifyName();
    qDebug() << "HttpStatusCodeAttribute : " << req->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << "HttpReasonPhraseAttribute : " << req->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
    qDebug() << "RedirectionTargetAttribute : " << req->attribute(QNetworkRequest::RedirectionTargetAttribute);
    qDebug() << "ConnectionEncryptedAttribute : " << req->attribute(QNetworkRequest::ConnectionEncryptedAttribute);
    qDebug() << "CacheLoadControlAttribute : " << req->attribute(QNetworkRequest::CacheLoadControlAttribute);
    qDebug() << "CacheSaveControlAttribute : " << req->attribute(QNetworkRequest::CacheSaveControlAttribute);
    qDebug() << "SourceIsFromCacheAttribute : " << req->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
    qDebug() << "DoNotBufferUploadDataAttribute : " << req->attribute(QNetworkRequest::DoNotBufferUploadDataAttribute);
    qDebug() << "HttpPipeliningAllowedAttribute : " << req->attribute(QNetworkRequest::HttpPipeliningAllowedAttribute);
    qDebug() << "HttpPipeliningWasUsedAttribute : " << req->attribute(QNetworkRequest::HttpPipeliningWasUsedAttribute);
    qDebug() << "CustomVerbAttribute : " << req->attribute(QNetworkRequest::CustomVerbAttribute);
    qDebug() << "CookieLoadControlAttribute : " << req->attribute(QNetworkRequest::CookieLoadControlAttribute);
    qDebug() << "CookieSaveControlAttribute : " << req->attribute(QNetworkRequest::CookieSaveControlAttribute);
    qDebug() << "AuthenticationReuseAttribute : " << req->attribute(QNetworkRequest::AuthenticationReuseAttribute);
    qDebug() << "BackgroundRequestAttribute : " << req->attribute(QNetworkRequest::BackgroundRequestAttribute);
    qDebug() << "EmitAllUploadProgressSignalsAttribute : " << req->attribute(QNetworkRequest::EmitAllUploadProgressSignalsAttribute);
    qDebug() << "OriginalContentLengthAttribute : " << req->attribute(QNetworkRequest::OriginalContentLengthAttribute);
    qDebug() << "RedirectPolicyAttribute : " << req->attribute(QNetworkRequest::RedirectPolicyAttribute);
    qDebug() << "Http2DirectAttribute : " << req->attribute(QNetworkRequest::Http2DirectAttribute);
    qDebug() << "AutoDeleteReplyOnFinishAttribute : " << req->attribute(QNetworkRequest::AutoDeleteReplyOnFinishAttribute);
}

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

            qDebug() << "\n\nRequest attributes.\n";

            auto req = reply->request();
            printRequestAttribute(&req);
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
