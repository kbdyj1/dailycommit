#ifndef REMOCON_H
#define REMOCON_H

#include <QObject>
#include "light_interface.h"

class Remocon : public QObject
{
    Q_OBJECT

public:
    explicit Remocon(QObject* parent = nullptr);

public slots:
    void turnOn();
    void turnOff();

    void testSlot(const QString&);

private:
    local::Light* light = nullptr;
};

#endif // REMOCON_H
