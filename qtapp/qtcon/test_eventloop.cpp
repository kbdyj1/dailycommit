#include <QEventLoop>
#include <QCoreApplication>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QDebug>

class TimerHandler : public QObject
{
    Q_OBJECT

public:
    TimerHandler(QObject* parent) : QObject(parent)
    {}
    virtual ~TimerHandler()
    {
        qDebug() << "~TimerHandler()";
    }

    virtual bool event(QEvent *ev) override
    {
        switch (ev->type()) {
        case QEvent::Timer:
            qDebug() << "event(QEvent::Timer)";
            callCount++;

            if (5 < callCount) {
                auto* app = dynamic_cast<QCoreApplication*>(this->parent());
                if (app) {
                    auto* dispatcher = app->eventDispatcher();
                    qDebug() << "unregisterTimer #1";
                    dispatcher->unregisterTimer(timerId);
                    qDebug() << "unregisterTimer #2";

                    timerId = -1;
                }
            }
            break;

        default:
            qDebug() << "event(" << ev->type() << ")";
            break;
        }
        return true;
    }

    void setTimerId(int id)
    {
        timerId = id;
    }

private:
    int timerId = -1;
    int callCount = 0;
};

void test_eventloop(QCoreApplication* app)
{
    auto* dispatcher = app->eventDispatcher();
    auto* timerHandler = new TimerHandler{app};

    auto timerId = dispatcher->registerTimer(1000, Qt::CoarseTimer, timerHandler);
    timerHandler->setTimerId(timerId);
}

#include "test_eventloop.moc"
