#include <QCoreApplication>
#include <QLocalSocket>
#include "LocalServer.h"

#define USE_LOCAL_SERVER

void startServer();
int startClient(int argc, char** argv);
void test_atomic();

void testLocalServer(int argc, char* argv[], QObject* parent)
{
    if (argc < 2) {
        fprintf(stdout, "nothing...\n");
    } else {
        if (0 == strncmp(argv[1], "s", 1)) {
    #ifdef USE_LOCAL_SERVER
            qDebug() << "start LocalServer...";
            auto* localServer = new LocalServer(parent);
            if (localServer->listen()) {
                qDebug() << "localServer->listen(): true";
            }
    #else
            startServer();
    #endif
        } else if (0 == strncmp(argv[1], "c", 1)) {
    #ifdef USE_LOCAL_SERVER
            qDebug() << "start QLocalSocket...";
            auto* localSocket = new QLocalSocket(parent);

            localSocket->connectToServer(LOCAL_SERVER_NAME);
            localSocket->waitForConnected();
    #else
            startClient(argc, argv);
    #endif
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    testLocalServer(argc, argv, &a);
#endif

    test_atomic();
    return a.exec();
}
