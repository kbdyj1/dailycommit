#ifndef STORE_H
#define STORE_H

#include <QVariant>

class Dispatcher;

class Store
{
public:
    void subscribe(Dispatcher *dispatcher);
    virtual void onNotification(int action, const QVariant &args);
};

#endif // STORE_H
