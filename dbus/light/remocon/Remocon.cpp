#include "Remocon.h"
#include <QDBusConnection>

Remocon::Remocon(QObject* parent) : QObject(parent)
{
    light = new local::Light("my.service.light", "/light", QDBusConnection::sessionBus(), this);
}

void Remocon::turnOn()
{
    light->turnOn();
}

void Remocon::turnOff()
{
    light->turnOff();
}
