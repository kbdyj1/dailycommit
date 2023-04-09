#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SequenceType.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/02_performance/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<SequenceType>("Performance.Example", 1, 0, "SequenceType");
    engine.load(url);



    return app.exec();
}
