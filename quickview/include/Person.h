#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QDebug>

class Person : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)

public:
    Person(QObject *parent = nullptr) : QObject(parent)
    {
        qDebug() << "Person()";
    }
    virtual ~Person()
    {
        qDebug() << "~Person()";
    }
    Person(const Person &other) {
        mName = other.name();
    }
    Person& operator=(const Person &other) {
        if (this != &other) {
            mName = other.name();
        }
        return *this;
    }

    QString name() const {
        return mName;
    }
    void setName(const QString &name) {
        mName = name;
    }

private:
    QString mName;
};

#endif // PERSON_H
