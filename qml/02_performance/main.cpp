#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SequenceType.h"
#include "CppLogicTest.h"
#include "CppModel.h"

#define RUN_AS_CONSOLE

void test_literal();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#ifndef RUN_AS_CONSOLE
    QQmlApplicationEngine engine;
    const QUrl url("qrc:/02_performance/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<SequenceType>("Performance.Example", 1, 0, "SequenceType");
    qmlRegisterType<CppModel>("CppModel.Example", 1, 0, "CppModel");

    engine.rootContext()->setContextProperty("cpp", new CppLogicTest(&engine));

    engine.load(url);
#else
    test_literal();
#endif

    return app.exec();
}
