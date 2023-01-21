#ifndef TESTSOCKET_H
#define TESTSOCKET_H

#include <QObject>
#include <QLocalSocket>

class TestSocket : public QObject
{
public:
    TestSocket(QObject* parent = nullptr) : QObject(parent)
    {}
    virtual ~TestSocket()
    {}

private:
    QLocalSocket* socket = nullptr;
};

#endif // TESTSOCKET_H
