#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QVariant>
#include "Action.h"

class ViewPrivate;

class View : public QObject
{
    Q_OBJECT

    Q_DECLARE_PRIVATE(View)

public:
    Q_INVOKABLE View(const QString &viewName, QObject *parent = nullptr);
    virtual ~View();

    Q_INVOKABLE virtual bool subscribe(int action, const QVariant &param = QVariant{})
    {
        return true;
    }

    void setQml(QObject *);
    void setQmlProperty(const char *propertyName, const QVariant &value);

private:
    Q_D(View);
};

#endif // VIEW_H
