#include <QQuickView>
#include <QQuickItem>

namespace internal
{

void testNullishCoalescing(QQuickView *view)
{
    auto *rootObject = view->rootObject();

    view->setSource(QUrl("qrc:/untitled/NullishCoalescing.qml"));
    QMetaObject::invokeMethod(rootObject, "testNullishCoalescing");
}

void testRequiredProperties(QQuickView *view)
{
    view->setSource(QUrl("qrc:/untitled/UseNewQmlLanguageFeature.qml"));
}

}//internal

void testNewQmlGrammer(QQuickView *view)
{
    //internal::testNullishCoalescing(view);
    internal::testRequiredProperties(view);
}
