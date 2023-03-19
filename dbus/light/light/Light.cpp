#include "Light.h"
#include "light_adaptor.h"

Light::Light(QObject* parent)
    : QObject(parent)
{
    new LightAdaptor(this);
}

bool Light::on() const
{
    return bOn;
}

void Light::setOn(bool b)
{
    if (bOn != b) {
        bOn = b;
        emit onChanged();
    }
}

void Light::turnOn()
{
    setOn(true);
}

void Light::turnOff()
{
    setOn(false);
}
