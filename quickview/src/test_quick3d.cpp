#include <QQuickView>
#include <QQuickItem>

namespace internal
{

void testSimple3D(QQuickView *view)
{
    view->setSource(QUrl("qrc:/quickview/qml/simple3d.qml"));
}

}

void testQuick3D(QQuickView *view)
{
    internal::testSimple3D(view);
}
