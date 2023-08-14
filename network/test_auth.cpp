#include <QtNetworkAuth>
#include <QDesktopServices>

class TestAuth : public QObject
{
    Q_OBJECT

public:
    TestAuth(QObject* parent) : QObject(parent)
    {
        const quint16 port = 1337;
        auto* handler = new QOAuthHttpServerReplyHandler(port, this);

        auto baseUrl = QString{"https://www.reddit.com/api/v1"};
        auto authorizeUrl = baseUrl + "/authorize";
        auto accessTokenUrl = baseUrl + "/access_token";
        oauth2.setReplyHandler(handler);
        oauth2.setAuthorizationUrl(QUrl{authorizeUrl});
        oauth2.setAccessTokenUrl(QUrl{accessTokenUrl});
        oauth2.setScope("identity read");

        connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [=](QAbstractOAuth::Status status){
            qDebug() << "QOAuth2AuthorizationCodeFlow::statusChanged()";

            if (status == QAbstractOAuth::Status::Granted) {
                emit authenticated();
            }
        });
        oauth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QVariantMap* param){
            if (stage == QAbstractOAuth::Stage::RequestingAuthorization && isPermanent())
                param->insert("duration", "permanent");
        });
        connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    }
    TestAuth(const QString& identifier, QObject* parent) : TestAuth(parent)
    {
        oauth2.setClientIdentifier(identifier);
    }
    virtual ~TestAuth()
    {}

    QNetworkReply* requestHotThreads()
    {
        qDebug() << "Getting hot threads...";
        const auto hotUrl = QUrl{"https://oauth.reddit.com/hot"};
        return oauth2.get(hotUrl);
    }

    void setPermanent(bool value)
    {
        permanent = value;
    }
    bool isPermanent() const
    {
        return permanent;
    }

public Q_SLOTS:
    void grant()
    {
        oauth2.grant();

        connect(this, &TestAuth::authenticated, this, &TestAuth::update);
    }
    void subscribeToLiveUpdates()
    {
        qDebug() << "Subscribing...";
        auto* reply = oauth2.get(QUrl{"https://oauth.reddit.com/live/XXXX/about.json"});
        connect(reply, &QNetworkReply::finished, [=](){
            if (reply->error() != QNetworkReply::NoError) {
                qCritical() << "Reddit error: " << reply->errorString();
                return;
            }

            const auto json = reply->readAll();
            const auto doc = QJsonDocument::fromJson(json);
            const auto root = doc.object();
            const auto data = root["data"].toObject();
            const auto websocketUrlValue = data.value("websocket_url");
            const QUrl websocketUrl{websocketUrlValue.toString()};

            emit subscribed(websocketUrl);
        });
    }
    void update()
    {
        auto* reply = requestHotThreads();
        connect(reply, &QNetworkReply::finished, [=](){
            if (reply->error() != QNetworkReply::NoError) {
                qCritical() << reply->errorString();
                return;
            }

            qDebug() << "update...";

            const auto json = reply->readAll();
            const auto doc = QJsonDocument::fromJson(json);
            const auto root = doc.object();
            const auto data = root["data"].toObject();
            const auto children = data["children"].toArray();

            for (const auto child : qAsConst(children)) {
                threads.append(child.toObject());
                qDebug() << child;
            }
        });
    }

Q_SIGNALS:
    void authenticated();
    void subscribed(const QUrl& url);

private:
    QOAuth2AuthorizationCodeFlow oauth2;
    bool permanent = false;
    QList<QJsonObject> threads;
};

namespace { //=================================================================

namespace _1 {

void test(QObject* parent, const QString& identifier)
{
    auto* testAuth = new TestAuth(identifier, parent);
    testAuth->grant();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_auth(QObject* parent, const QString& identifier)
{
    _1::test(parent, identifier);
}

#include "test_auth.moc"
