#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SystemDispatcher.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    auto *dispatcher = new SystemDispatcher(&engine, &engine);
    engine.rootContext()->setContextProperty("dispatcher", dispatcher);

    qmlRegisterType<Action>("toy.core", 1, 0, "Action");

    engine.load(url);

    return app.exec();
}
