#include <QQuickView>
#include "Person.h"
#include "BirthdayParty.h"

void test_defaultProperty(QQuickView *view, const QString &url)
{
    qmlRegisterType<Person>("Birthday", 1, 0, "Person");
    qmlRegisterType<BirthdayParty>("Birthday", 1, 0, "BirthdayParty");

    view->setSource(url);
}
