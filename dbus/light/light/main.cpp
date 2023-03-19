#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDBusConnection>

#include "Light.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Light light;

    engine.rootContext()->setContextProperty("light", &light);

    auto sessionBus = QDBusConnection::sessionBus();
    sessionBus.registerObject("/light", &light);
    sessionBus.registerService("my.service.light");

    const QUrl url(u"qrc:/light/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
