#include <QQuickView>
#include <QQuickItem>

namespace internal
{

void testLocalStorageSample(QQuickView *view)
{
    view->setSource(QUrl("qrc:/quickview/qml/LocalStorageSample.qml"));
}

}

void testLocalStorage(QQuickView *view)
{
    internal::testLocalStorageSample(view);
}
