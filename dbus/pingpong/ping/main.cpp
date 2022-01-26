#include <QCoreApplication>
#include <QtDBus>
#include <QDebug>

#include "../common.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Ping: Can't connect to the D-Bus session bus.";
        return 1;
    }

    QDBusInterface iface(SERVICE_NAME, "/", "", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusReply<QString> reply = iface.call("ping", "hello, Qt");
        if (reply.isValid()) {
            qDebug() << "reply: " << qPrintable(reply.value());
            return 0;
        } else {
            qDebug() << "call failed: " << qPrintable(reply.error().message());
            return 1;
        }
    } else {
        qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    return a.exec();
}
