#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Rectangle.h"
#include <QDebug>
#include <QQuickView>
#include <QtWebView>

/*
 * Qt property binding example
 *
 */
void launchGuiApplication(QGuiApplication &app) {

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/quickview/qml/main.qml"_qs);
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
void testStateMachine(QQuickView *view);
void testJsValue();
void testJsEngine();
void testJavaScript(QQuickView *view);
void test_http(QQuickView *view);
void testLocalStorage(QQuickView *view);
void testSimpleQml(QQuickView *view, const QString &url);

void launchQuickView() {
    QQuickView *view = new QQuickView;    

    //testConversionBetweenQtAndJavascriptTypes(view);
    //testNestedItem(view);
    //testNewQmlGrammer(view);
    //testQuick3D(view);
    //testStateMachine(view);
    //testJsValue();
    //testJsEngine();
    //testJavaScript(view);
    //test_http(view);
    //testLocalStorage(view);
    testSimpleQml(view, "qrc:/quickview/qml/TestInput.qml");

    view->show();
}

int mainQuickView(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qputenv("QT_QPA_PLATFORM", "offscreen");

    QtWebView::initialize();

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
