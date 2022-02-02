#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>

class QVariant;

class Dispatcher : public QObject
{
    Q_OBJECT

public:
    Dispatcher(QObject *parent = nullptr) : QObject(parent)
    {}
    virtual ~Dispatcher()
    {}

    Q_INVOKABLE virtual void dispatch(int action, const QVariant &param) = 0;
};

#endif // DISPATCHER_H
