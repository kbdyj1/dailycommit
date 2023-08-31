#include <QThread>
#include <QThreadPool>
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

namespace _2 {

class HelloQtTask : public QRunnable
{
    virtual~HelloQtTask()
    {
        qDebug() << "~HelloQtTask()";
    }
    void run() override
    {
        qDebug() << "Hello, Qt";
    }
};

void test()
{
    QThreadPool* threadPool = QThreadPool::globalInstance();

    qDebug() << "activeThreadCount: " << threadPool->activeThreadCount();
    qDebug() << "expiryTimeout: " << threadPool->expiryTimeout();
    qDebug() << "maxThreadCount: " << threadPool->maxThreadCount();
    qDebug() << "stackSize: " << threadPool->stackSize();

    threadPool->start(new HelloQtTask());
//    auto func = std::function<void()>([]{
//        qDebug() << "std::function<()>";
//    });
//    threadPool->start(func);
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_thread(QObject* parent)
{
    Q_UNUSED(parent)

#if (0) //done
    _1::test(parent);
#endif

    _2::test();
}

#include "test_thread.moc"
