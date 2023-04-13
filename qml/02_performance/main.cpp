#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SequenceType.h"
#include "CppLogicTest.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/02_performance/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<SequenceType>("Performance.Example", 1, 0, "SequenceType");

    engine.rootContext()->setContextProperty("cpp", new CppLogicTest(&engine));

    engine.load(url);

    return app.exec();
}
