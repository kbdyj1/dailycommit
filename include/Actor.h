#ifndef ACTOR_H
#define ACTOR_H

#include <QString>
#include <QMetaType>
#include <QQmlEngine>

class Actor
{
    Q_GADGET
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(Status status READ status WRITE setStatus)

public:
    enum Status {
        Ready,
        Loading,
        Error
    };
    Q_ENUM(Status)

    QString name() const {
        return mName;
    }
    void setName(const QString &name) {
        mName = name;
    }
    Status status() const {
        return mStatus;
    }
    void setStatus(Status status) {
        mStatus = status;
    }
    static void declareToQml() {
        qmlRegisterType<Actor>("Actor", 1, 0, "Actor");
    }

private:
    QString mName;
    Status mStatus = Ready;
};
Q_DECLARE_METATYPE(Actor)

#endif // ACTOR_H
