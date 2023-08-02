#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

void test_get(QObject* parent);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test_get(&a);

    return a.exec();
}
