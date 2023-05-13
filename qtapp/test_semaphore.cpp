#include <QSemaphore>
#include <QDebug>
#include <QTimer>

namespace { //=================================================================

namespace _1 {

QSemaphore sem(4);

void test()
{
    qDebug() << "sem(4).available(): " << sem.available();

    qDebug() << "sem.acquire()";
    sem.acquire();

    const int acquireCount = 4;
    const int timeoutMilliseconds = 2000;

    qDebug() << "before sem.tryAcquire(" << acquireCount << ")";
    auto enable = sem.tryAcquire(acquireCount, timeoutMilliseconds);

    if (enable) {
        qDebug() << "after sem.tryAcquire(" << acquireCount << ")";
    } else {
        qDebug() << "sem.tryAcquire(" << acquireCount << ") failed.";
    }
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_semaphore()
{
    _1::test();
}
