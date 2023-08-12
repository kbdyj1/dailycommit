#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QAbstractNetworkCache>
#include <QStandardPaths>
#include <QTimer>
#include <QFile>

#define TEST_SAVE_RESPONSE

namespace { //=================================================================

namespace _1 {

constexpr const char* TEST_URL = "https://doc.qt.io/qt-6/qnetworkdiskcache.html";

class Test : public QObject
{
    Q_OBJECT

public:
    Test(QObject* parent = nullptr) : QObject(parent)
    {
        auto cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cacheDir/";
        auto* diskCache = new QNetworkDiskCache(this);
        diskCache->setCacheDirectory(cacheDir);

        qDebug() << "cache dir: " << cacheDir;

        nam.setCache(diskCache);
    }

    void request(const QUrl& url)
    {
        auto* req = new QNetworkRequest(url);
        req->setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

        auto* reply = nam.get(*req);
        connect(reply, &QNetworkReply::finished, this, &Test::onFinished);
    }

public Q_SLOTS:
    void onTimer()
    {
        request(QUrl{TEST_URL});
    }

protected Q_SLOTS:
    void onFinished()
    {
        auto* reply = dynamic_cast<QNetworkReply*>(sender());
        {
            auto fromCache = reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
            qDebug() << "from cache? " << fromCache;

            auto url = reply->url();
            auto* cache = nam.cache();
            if (cache) {
                qDebug() << "cache size: " << cache->cacheSize();

                auto* io = cache->data(url);
                if (io) {
                    qDebug() << "cache readable? " << io->isReadable();
                    auto responseFromCache = io->readAll();

#ifdef TEST_SAVE_RESPONSE
                    QFile file{"cache.html"};
                    if (file.open(QFile::WriteOnly)) {
                        file.write(responseFromCache);
                        file.close();
                    }
#endif
                } else {
                    qDebug() << "cache data: nullptr";
                }
            } else {
                qDebug() << "cache: nullptr";
            }
        }
    }

private:
    QNetworkAccessManager nam;
};

void test(QObject* parent)
{
    auto* inst = new Test(parent);
    if (inst) {
        inst->request(QUrl{TEST_URL});
    }

    QTimer::singleShot(2000, inst, &Test::onTimer);
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_cache(QObject* parent)
{
    _1::test(parent);
}

#include "test_cache.moc"
