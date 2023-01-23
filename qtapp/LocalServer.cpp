#include "LocalServer.h"
#include <QDebug>

LocalServer::LocalServer(QObject *parent) : QObject(parent)
{
    server = new QLocalServer(parent);

    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

bool LocalServer::listen()
{
    auto ret = server->listen(LOCAL_SERVER_NAME);
    if (!ret) {
        qDebug() << server->errorString();
    }
    return ret;
}

void LocalServer::onNewConnection()
{
    qDebug() << "New connection";
}
