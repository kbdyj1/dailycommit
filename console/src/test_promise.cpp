#include <QPromise>
#include <QFuture>

namespace { //=================================================================

void test0()
{
    auto promise = QPromise<int>();
    auto future = promise.future();
    auto thread = QScopedPointer<QThread>(
        QThread::create([](QPromise<int> promise){
            promise.start();
            promise.addResult(40);
            promise.finish();
        }, std::move(promise))
    );
    thread->start();

    future.waitForFinished();
    qDebug() << "future.result()" << future.result();
}

void test1()
{
    auto sharedPromise = QSharedPointer<QPromise<int>>(new QPromise<int>());
    auto future = sharedPromise->future();

    QScopedPointer<QThread> threads[] = {
        QScopedPointer<QThread>(QThread::create([](auto sharedPromise){
            sharedPromise->addResult(0, 0);
        }, sharedPromise)),
        QScopedPointer<QThread>(QThread::create([](auto sharedPromise){
            sharedPromise->addResult(1, 1);
        }, sharedPromise)),
        QScopedPointer<QThread>(QThread::create([](auto sharedPromise){
            sharedPromise->addResult(2, 2);
        }, sharedPromise))
    };
    for (auto &t : threads) {
        t->start();
    }

    qDebug() << "[0] : " << future.resultAt(0);
    qDebug() << "[1] : " << future.resultAt(1);
    qDebug() << "[2] : " << future.resultAt(2);
}

void test2()
{
    auto promise = QPromise<int>{};
    auto future = promise.future();

    promise.start();
    auto thread = QScopedPointer<QThread>(QThread::create([](QPromise<int> promise){
        for (int i=0; i<100; i++) {
            promise.addResult(i);
            promise.suspendIfRequested();
            if (promise.isCanceled()) {
                qDebug() << "   [thread] canceled...";
                break;
            }
        }
        promise.finish();
    }, std::move(promise)));

    thread->start();

    future.suspend();
    qDebug() << "[Suspend] result count: " << future.resultCount();

    future.resume();
    qDebug() << "[Resume] result count: " << future.resultCount();

    int count = 0;
    while (future.isFinished() == false) {
        qDebug() << "[while " << count << "] result count: " << future.resultCount();
        ++count;

        if (count == 3) {
            future.cancel();
            break;
        }
    }
    thread->wait(1000);
    future.waitForFinished();

    qDebug() << "[Finished] result count: " << future.resultCount();
}

} // namespace ================================================================

void test_promise()
{
    //test0();
    //test1();
    test2();
}
