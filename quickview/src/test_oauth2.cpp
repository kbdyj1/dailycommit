#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>

#define SPOTIFY_CLIENT_ID       ""
#define SPOTIFY_CLIENT_SECRET   ""

class Spotify : public QObject
{
    Q_OBJECT

public:
    Spotify(QObject *parent = nullptr) : QObject(parent)
    {
        mOAuth2.setAuthorizationUrl(QUrl{"https://accounts.spotify.com/authorize"});
        mOAuth2.setAccessTokenUrl(QUrl{"https://accounts.spotify.com/api/token"});
        mOAuth2.setScope("user-top-read");

        mOAuth2.setReplyHandler(new QOAuthHttpServerReplyHandler{8000, this});
        mOAuth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *params){
            if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
                params->insert("duration", "permanent");
            }
        });

        mOAuth2.setClientIdentifier(SPOTIFY_CLIENT_ID);
        mOAuth2.setClientIdentifierSharedKey(SPOTIFY_CLIENT_SECRET);

        connect(&mOAuth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](const QUrl &url){
            qDebug() << url;
            QDesktopServices::openUrl(url);
        });
        connect(&mOAuth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [=](QAbstractOAuth::Status status){
            if (status == QAbstractOAuth::Status::Granted) {
                setAuthenticated(true);
            } else {
                setAuthenticated(false);
            }
        });
    }

    void setAuthenticated(bool b) {
        if (mAuthenticated != b) {
            mAuthenticated = b;
            emit authenticatedChanged();

            qDebug() << "authenticated:" << b;
        }
    }

    void authorize() {
        mOAuth2.grant();
    }

Q_SIGNALS:
    void authenticatedChanged();

private:
    QOAuth2AuthorizationCodeFlow mOAuth2;
    bool mAuthenticated = false;
};

void test_oauth2()
{
    auto spotify = QScopedPointer<Spotify>(new Spotify);
    spotify->authorize();
}

#include "test_oauth2.moc"
