#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

void test_get(QObject* parent);
void test_cache(QObject* parent);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_get(&a);
#endif

    test_cache(&a);

    return a.exec();
}
