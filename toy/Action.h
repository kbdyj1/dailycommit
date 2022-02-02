#ifndef ACTION_H
#define ACTION_H

#include <QObject>

class Action
{
    Q_GADGET

public:
    enum Id {
        RegisterRoot,
        CreateView,

        Created,

        User = 10000,
    };
    Q_ENUM(Id)
};

#endif // ACTION_H
