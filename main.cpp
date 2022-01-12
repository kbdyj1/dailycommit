#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Rectangle.h"
#include <QDebug>
#include <QQuickView>
#include <QQuick3D>

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

void testConversionBetweenQtAndJavascriptTypes(QQuickView *view);
void testNestedItem(QQuickView *view);
void testKDBindings();
void testNewQmlGrammer(QQuickView *view);
void testQuick3D(QQuickView *view);

/*
 * Conversion between Qt and JavaScript Types
 *
 */
void launchQuickView() {
    QQuickView *view = new QQuickView;    

    //testConversionBetweenQtAndJavascriptTypes(view);
    //testNestedItem(view);
    //testKDBindings();
    //testNewQmlGrammer(view);
    testQuick3D(view);

    view->show();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    //qputenv("QT_QUICK_BACKEND", "software");

#if (0)
    launchGuiApplication(app);
#else
    launchQuickView();
#endif
    return app.exec();
}
