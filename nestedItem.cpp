#include <QQuickView>
#include <QQuickItem>

void testNestedItem(QQuickView *view)
{
    view->setSource(QUrl("qrc:/untitled/nested.qml"));
}
