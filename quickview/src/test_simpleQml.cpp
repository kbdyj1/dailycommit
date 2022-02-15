#include <QQuickView>

void testSimpleQml(QQuickView *view, const QString &url)
{
    view->setSource(url);
}
