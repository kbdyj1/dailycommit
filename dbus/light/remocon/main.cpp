#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Remocon.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Remocon remocon;

    engine.rootContext()->setContextProperty("remocon", &remocon);

    const QUrl url(u"qrc:/remocon/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
