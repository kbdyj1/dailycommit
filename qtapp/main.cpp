#include <QCoreApplication>
#include "TestSocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto* testSocket = new TestSocket{};

    return a.exec();
}
