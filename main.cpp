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
void testKDBindings();
void testNewQmlGrammer(QQuickView *view);
void testQuick3D(QQuickView *view);

void launchQuickView() {
    QQuickView *view = new QQuickView;    

    //testConversionBetweenQtAndJavascriptTypes(view);
    //testNestedItem(view);
    //testKDBindings();
    //testNewQmlGrammer(view);
    testQuick3D(view);

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

/*
 * Qt console test codes
 *
 */
void testStringView();

int mainConsole(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    testStringView();

    return 0;
}

int main(int argc, char *argv[])
{
#if (0)
    return mainQuickView(argc, argv);
#else
    return mainConsole(argc, argv);
#endif
}
