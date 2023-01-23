#include <QCoreApplication>
#include <QLocalSocket>
#include "LocalServer.h"

#define USE_LOCAL_SERVER

void startServer();
int startClient(int argc, char** argv);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        fprintf(stdout, "nothing...\n");
    } else {
        if (0 == strncmp(argv[1], "s", 1)) {
#ifdef USE_LOCAL_SERVER
            qDebug() << "start LocalServer...";
            auto* localServer = new LocalServer(&a);
            if (localServer->listen()) {
                qDebug() << "localServer->listen(): true";
            }
#else
            startServer();
#endif
        } else if (0 == strncmp(argv[1], "c", 1)) {
#ifdef USE_LOCAL_SERVER
            qDebug() << "start QLocalSocket...";
            auto* localSocket = new QLocalSocket(&a);

            localSocket->connectToServer(LOCAL_SERVER_NAME);
            localSocket->waitForConnected();
#else
            startClient(argc, argv);
#endif
        }
    }

    return a.exec();
}
