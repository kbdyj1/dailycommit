#ifndef BIG_H
#define BIG_H

#include <QObject>

class Big : public QObject
{
    Q_OBJECT

public:
    Big();

    void write(const QString& message);
};

#endif // BIG_H
