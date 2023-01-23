#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QLocalServer>

#define LOCAL_SERVER_NAME   "localServer2"

class LocalServer : public QObject{
    Q_OBJECT

public:
    LocalServer(QObject* parent);

    bool listen();

    virtual ~LocalServer()
    {
        server->close();
    }

public slots:
    void onNewConnection();

private:
    QLocalServer* server;
};

#endif // LOCALSERVER_H
