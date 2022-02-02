#ifndef SYSTEMDISPATCHER_H
#define SYSTEMDISPATCHER_H

#include "Dispatcher.h"
#include "Action.h"

class QQmlEngine;
class SystemDispatcherPrivate;

class SystemDispatcher : public Dispatcher, public Action
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SystemDispatcher)

public:
    SystemDispatcher(QQmlEngine *engine, QObject *parent = nullptr);
    virtual ~SystemDispatcher();

    virtual void dispatch(int action, const QVariant &param) override;

private:
    Q_D(SystemDispatcher);
};

#endif // SYSTEMDISPATCHER_H
