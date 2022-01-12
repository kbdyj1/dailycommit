#include <QQuickView>
#include <QQuickItem>

namespace internal
{

void testSimple3D(QQuickView *view)
{
    auto *rootObject = view->rootObject();

    view->setSource(QUrl("qrc:/untitled/simple3d.qml"));
}

}

void testQuick3D(QQuickView *view)
{
    internal::testSimple3D(view);
}
