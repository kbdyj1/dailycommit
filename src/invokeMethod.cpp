#include <QQuickView>
#include <QQuickItem>
#include <QObject>
#include <QVariant>
#include <QColor>
#include <QDate>
#include "Actor.h"

namespace internal
{

/*
 * deliver Qt sequence to qml
 *
 */
void testQtSequenceType(QObject *rootObject)
{
    QVariantList list;
    list << 10 << QColor(Qt::green) << "botle";

    QVariantMap map;
    map["language"] = "QML";
    map["released"] = QDate(2022,1,8);

    QMetaObject::invokeMethod(rootObject,
        "read",
        Q_ARG(QVariant, QVariant::fromValue(list)),
        Q_ARG(QVariant, QVariant::fromValue(map)));
}

/*
 * test QDateTime
 *
 */
void testQtDateTimeType(QObject *rootObject)
{
    auto dt = QDateTime::currentDateTime();
    auto ret = QVariant{};

#if (1)
    QMetaObject::invokeMethod(rootObject,
        "readDate",
        Q_RETURN_ARG(QVariant, ret),
        Q_ARG(QVariant, QVariant::fromValue(dt)));

    qDebug() << "qml ret : " << ret.value<QDateTime>();
#else
    QVariant v{dt};
    qDebug() << "canConvert<QDateTime>() : " << v.canConvert<QDateTime>();
#endif
}

/*
 * Test GADGET
 *
 */
void testQtGadget(QObject *rootObject)
{
    Actor actor;
    actor.setName("devilqoo");
    actor.setStatus(Actor::Loading);

    QMetaObject::invokeMethod(rootObject, "readActor", Q_ARG(QVariant, QVariant::fromValue(actor)));
}

} //namespace

void testConversionBetweenQtAndJavascriptTypes(QQuickView *view)
{
    //Register the Action (eg. in main(), before creating a Qml View/Context):
    Actor::declareToQml();

    view->setSource(QUrl("qrc:/untitled/main.qml"));

    auto *rootObject = view->rootObject();

    internal::testQtSequenceType(rootObject);
    internal::testQtDateTimeType(rootObject);
    internal::testQtGadget(rootObject);
}
