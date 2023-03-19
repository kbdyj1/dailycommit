#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>

class Light : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool on READ on WRITE setOn NOTIFY onChanged)

public:
    explicit Light(QObject* parent = nullptr);

    bool on() const;
    void setOn(bool);

signals:
    void onChanged();

public slots:
    void turnOn();
    void turnOff();

private:
    bool bOn = false;
};

#endif // LIGHT_H
