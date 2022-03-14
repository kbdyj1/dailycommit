#ifndef BIRTHDAYPARTY_H
#define BIRTHDAYPARTY_H

#include <QObject>
#include <QQmlListProperty>
#include "Person.h"

class Person;

class BirthdayParty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Person *host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QQmlListProperty<Person> guests READ guests)
    Q_CLASSINFO("DefaultProperty", "guests")

public:
    BirthdayParty(QObject *parent = nullptr) : QObject(parent)
    {}

    Person *host() const {
        return mHost;
    }
    void setHost(Person *host) {
        if (mHost != host) {
            mHost = host;

            qDebug() << "setHost(" << host->name() << ")";

            emit hostChanged();
        }
    }

    QQmlListProperty<Person> guests() {
        auto list = QQmlListProperty<Person>(this, &mGuests);
        qDebug() << "guests(" << mGuests.count() << ")";
        return list;
    }

signals:
    void hostChanged();

private:
    Person *mHost;
    QList<Person *> mGuests;
};

#endif // BIRTHDAYPARTY_H
