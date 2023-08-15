#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

void test_get(QObject* parent);
void test_cache(QObject* parent);
void test_auth(QObject* parent, const QString& identifier);

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

#if (0) //done
    test_cache(&a);
    test_auth(&a, argv[1]);
#endif

    test_get(&a);

    return a.exec();
}
