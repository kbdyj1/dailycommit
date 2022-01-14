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

/*
 * Qt QuickView test codes
 *
 */
void testConversionBetweenQtAndJavascriptTypes(QQuickView *view);
void testNestedItem(QQuickView *view);
void testNewQmlGrammer(QQuickView *view);
void testQuick3D(QQuickView *view);
void testKDBindings();
void testStringView();

void launchQuickView() {
    QQuickView *view = new QQuickView;    

    //testConversionBetweenQtAndJavascriptTypes(view);
    //testNestedItem(view);
    //testKDBindings();
    //testNewQmlGrammer(view);
    //testQuick3D(view);
    testStringView();

    view->show();
}

int mainQuickView(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qputenv("QT_QPA_PLATFORM", "offscreen");

#if (0)
    launchGuiApplication(app);
#else
    launchQuickView();
#endif
    return app.exec();
}

int main(int argc, char *argv[])
{
    return mainQuickView(argc, argv);
}
