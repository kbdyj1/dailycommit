#include <QNetworkAccessManager>
#include <QQuickView>
#include <QQuickItem>
#include <QObject>
#include <QNetworkReply>
#include <QFile>
#include <QString>

class HttpTest : public QObject
{
    Q_OBJECT

public:
    HttpTest(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

public Q_SLOTS:
    void requestDownload(const QString &url, const QString &downloadPath) {
        qDebug() << "requestDownload(" << url << "," << downloadPath << ")";

        if (url.isEmpty() || downloadPath.isEmpty())
            return;

        mDownloadPath = downloadPath;
        mIsAppend = false;

        if (url.startsWith("http://") || url.startsWith("https://")) {
            get(url);
        } else {
            auto search = QString{"https://google.com/search?q=%1&oq=%1"}.arg(url);
            get(search);
        }
    }
    void onHttpFinished()
    {
        qDebug() << "finished";
        auto *reply = dynamic_cast<QNetworkReply*>(sender());
        reply->deleteLater();
    }
    void onReadyRead()
    {
        qDebug() << "ready read";
        auto *reply = dynamic_cast<QNetworkReply*>(sender());
        if (reply) {
            QFile file(mDownloadPath);
            auto openMode = (!mIsAppend) ? QFile::WriteOnly : (QFile::WriteOnly | QFile::Append);
            if (file.open(openMode)) {
                file.write(reply->readAll());
            }
            mIsAppend = true;
        }
    }
    void onErrorOccured(QNetworkReply::NetworkError error) {
        Q_UNUSED(error)

        auto *reply = dynamic_cast<QNetworkReply*>(sender());
        if (reply) {
            qDebug() << "error occured:" << reply->errorString();
        }
    }

private:
    void get(const QUrl &url)
    {
        auto *reply = mNetworkAccessManager.get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::finished, this, &HttpTest::onHttpFinished);
        connect(reply, &QIODevice::readyRead, this, &HttpTest::onReadyRead);
        connect(reply, &QNetworkReply::errorOccurred, this, &HttpTest::onErrorOccured);
    }

private:
    QNetworkAccessManager mNetworkAccessManager;
    QString mDownloadPath;
    bool mIsAppend;
};

void test_http(QQuickView *view)
{
    view->setSource(QUrl("qrc:/quickview/qml/HttpWindow.qml"));

    auto *rootObject = view->rootObject();

    auto *handler = new HttpTest(view);

    QObject::connect(rootObject, SIGNAL(startDownloadAt(QString,QString)), handler, SLOT(requestDownload(QString,QString)));
}

#include "test_http.moc"
