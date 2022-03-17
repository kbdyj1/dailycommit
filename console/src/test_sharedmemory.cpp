#include <QSharedMemory>
#include <QDebug>

namespace { //=================================================================

// Windows : QSharedMemory does not own the shared memory segment.
// Linux : QSharedMemory owns the shared memory segment.
// HP-UX : Only one attach to a shared memory segment is allowed per process.
// Apple platforms : require the use of POSIX shared memory

void test0()
{
    const auto key = QString{"dailycommit.test.sharedMemory"};
#if (0)
    auto shm = QSharedMemory{key};
#else
    auto shm = QSharedMemory{};
    shm.setKey(key);
#endif

    if (!shm.create(4) && shm.error() == QSharedMemory::AlreadyExists) {
        qDebug() << "[slave] QSharedMemory(" << key << ") :" << shm.errorString();
        shm.attach(QSharedMemory::ReadOnly);
        shm.lock();
        const quint32 *data = (quint32*)shm.constData();
        shm.unlock();
        qDebug() << "value: " << *data;
        qDebug() << "[slave] shm.detach(): " << shm.detach();
    } else {
        qDebug() << "[master] QSharedMemory(" << key << ") created.";

        qDebug() << "[master] native key: " << shm.nativeKey();
        // /tmp/qipc_sharedmemory_dailycommittestsharedMemory510eaab0953287b807a1bdd9f6fd38cf9e834259

        shm.lock();
        quint32 *data = (quint32*)shm.data();
        *data = 2023;
        shm.unlock();

        sleep(10);
        qDebug() << "[master] shm.detach(): " << shm.detach();
    }
}

} // namespace ================================================================

void test_sharedMemory()
{
    test0();
}
