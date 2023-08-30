#include <QThread>
#include <QDebug>
#include <QCoreApplication>

class ThreadMonitor : public QObject
{
    Q_OBJECT

public:
    ThreadMonitor(QObject* parent = nullptr) : QObject(parent)
    {}
    virtual ~ThreadMonitor()
    {}

    void run()
    {
        if (!thread) {
            thread = QThread::create([this](){
                while (!threadExit) {
                    QThread::msleep(1);
                    ++threadCount;

                    if (0 == threadCount % 1000) {
                        qDebug() << "threadCount: " << threadCount;
                    }
                    if (5000 == threadCount) {
                        break;
                    }
                }
            });

            connect(thread, &QThread::finished, [](){
                qDebug() << "QThread::finished caught!!!";
                qApp->quit();
            });
            thread->start();
        }
    }

private:
    QThread* thread = nullptr;
    bool threadExit = false;
    int threadCount = 0;
};

namespace { //=================================================================

namespace _1 {

void test(QObject* parent)
{
    auto* threadMonitor = new ThreadMonitor(parent);

    threadMonitor->run();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_thread(QObject* parent)
{
    _1::test(parent);
}

#include "test_thread.moc"
