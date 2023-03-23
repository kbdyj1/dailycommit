#include "Remocon.h"
#include <QDBusConnection>

#include <QDebug>

Remocon::Remocon(QObject* parent) : QObject(parent)
{
    light = new local::Light("my.service.light", "/light", QDBusConnection::sessionBus(), this);

    QDBusConnection::sessionBus().connect(QString(), QString(), "local.Light", "onChanged", this, SLOT(testSlot(QString)));
}

void Remocon::turnOn()
{
    light->turnOn();
}

void Remocon::turnOff()
{
    light->turnOff();
}

void Remocon::testSlot(const QString&)
{
    qDebug() << "Remocon::testSlot() !!!";
}
