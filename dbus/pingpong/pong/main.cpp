#include <QCoreApplication>
#include <QtDBus>

#include "../common.h"

class Pong : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    Q_SCRIPTABLE QString ping(const QString &message)
    {
        QMetaObject::invokeMethod(qApp, "quit");
        return QString("ping(%1) got called using Qt DBus").arg(message);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Pong: Can't connect to the D-Bus session bus.";
        return 1;
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
        qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        exit(1);
    }

    Pong pong;
    QDBusConnection::sessionBus().registerObject("/", &pong, QDBusConnection::ExportAllSlots);

    return a.exec();
}

#include "main.moc"
