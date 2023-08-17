#include <QNetworkAccessManager>
#include <QScopedPointer>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

class Test : public QObject
{
    Q_OBJECT

public:
    Test(QObject* parent)
        : QObject(parent)
        , nam(new QNetworkAccessManager(this))
    {}
    virtual ~Test()
    {}

    void printDebugMessage()
    {
        qDebug() << "autoDeleteReplies? " << nam->autoDeleteReplies();
        qDebug() << "supproted schemes: " << nam->supportedSchemes();
    }

private:
    QScopedPointer<QNetworkAccessManager> nam;
};

void test(QObject* parent)
{
    auto* net = new Test(parent);

    auto* metaObject = net->metaObject();
    auto classCount = metaObject->classInfoCount();
    qDebug() << "classInfo: " << classCount;
    qDebug() << "className: " << metaObject->className();

    net->printDebugMessage();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_network(QObject* parent)
{
    _1::test(parent);
}

#include "test_network.moc"
