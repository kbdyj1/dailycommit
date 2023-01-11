#include <QQuickView>
#include <QQuickItem>

namespace internal
{

void testSimple3D(QQuickView *view)
{
    view->setSource(QUrl("qrc:/quickview/qml/simple3d.qml"));
    auto roots = view->rootObject();

    auto view3d = roots->findChild<QObject*>(QString{"view3d"});
    if (view3d) {
        qDebug() << "w: " << view3d->property("width").toInt()
                 << ", h: " << view3d->property("height").toInt();
    }

    auto viewVisible = view->isVisible();
    qDebug() << "quickView.visible: " << viewVisible;
}

} //internal --------------------------------------------------------

void testQuick3D(QQuickView *view)
{
    internal::testSimple3D(view);
}
