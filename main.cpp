#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Rectangle.h"
#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QDate>

#include "Actor.h"

/*
 * Qt property binding example
 *
 */
void launchGuiApplication(QGuiApplication &app) {

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/untitled/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    Rectangle rect;

    auto notifier = rect.bindableArea().addNotifier([&]{
        qDebug() << "area: " << rect.area();
    });

    Qt::beginPropertyUpdateGroup();
    rect.setWidth(30);
    rect.setHeight(30);
    Qt::endPropertyUpdateGroup();
}

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
    QDateTime ret;

#if (0) // COMPILE ERROR !!! 22/01/08
    QMetaObject::invokeMethod(view->rootObject(),
        "readDate",
        Q_RETURN_ARG(QVariant, ret), // ???
        Q_ARG(QVariant, QVariant::fromValue(dt)));

    qDebug() << "qml ret : " << ret;
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

/*
 * Conversion between Qt and JavaScript Types
 *
 */
void launchQuickView() {
    QQuickView *view = new QQuickView;

    //Register the Action (eg. in main(), before creating a Qml View/Context):
    Actor::declareToQml();

    view->setSource(QUrl("qrc:/untitled/main.qml"));

    auto rootObject = view->rootObject();

    testQtSequenceType(rootObject);
    testQtDateTimeType(rootObject);
    testQtGadget(rootObject);

    view->show();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
#if (0)
    launchGuiApplication(app);
#else
    launchQuickView();
#endif
    return app.exec();
}
