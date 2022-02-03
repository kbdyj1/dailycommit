#include "View.h"
#include <QUrl>
#include <QQmlComponent>

class ViewPrivate
{
public:
    ViewPrivate(View *q) : q(q)
    {}
    QString mViewName;
    QObject *mQml;

private:
    View *q;
};

View::View(const QString &viewName, QObject *parent)
    : QObject(parent)
    , d(new ViewPrivate(this))
{
    d->mViewName = viewName;
}

View::~View()
{
    delete d;
}

void View::setQml(QObject *o)
{
    d->mQml = o;
}

void View::setQmlProperty(const char *propertyName, const QVariant &value)
{
    if (d->mQml) {
        d->mQml->setProperty(propertyName, value);
    }
}
