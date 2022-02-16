#include <QAtomicInteger>
#include <QDebug>

namespace internal
{

void test_atomic_features_using_macro()
{
//***************************************************************************** FETCH_AND_ADD
#if defined(Q_ATOMIC_INT32_FETCH_AND_ADD_IS_ALWAYS_NATIVE)
    qDebug() << "Q_ATOMIC_INT8_FETCH_AND_ADD_IS_ALWAYS_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_FETCH_AND_ADD_IS_NOT_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_ADD_IS_NOT_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_ADD_IS_SOMETIMES_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_FETCH_AND_ADD_IS_WAIT_FREE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_ADD_IS_WAIT_FREE";
#endif

//***************************************************************************** FETCH AND STORE
#if defined(Q_ATOMIC_INT32_FETCH_AND_STORE_IS_ALWAYS_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_STORE_IS_ALWAYS_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_FETCH_AND_STORE_IS_NOT_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_STORE_IS_NOT_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_STORE_IS_SOMETIMES_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_FETCH_AND_STORE_IS_WAIT_FREE)
    qDebug() << "Q_ATOMIC_INT32_FETCH_AND_STORE_IS_WAIT_FREE";
#endif

//***************************************************************************** REFERENCE COUTING
#if defined(Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_ALWAYS_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_ALWAYS_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_NOT_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_NOT_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_WAIT_FREE)
    qDebug() << "Q_ATOMIC_INT32_REFERENCE_COUNTING_IS_WAIT_FREE";
#endif

//***************************************************************************** TEST AND SET
#if defined(Q_ATOMIC_INT32_TEST_AND_SET_IS_ALWAYS_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_TEST_AND_SET_IS_ALWAYS_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_TEST_AND_SET_IS_NOT_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_TEST_AND_SET_IS_NOT_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_TEST_AND_SET_IS_SOMETIMES_NATIVE)
    qDebug() << "Q_ATOMIC_INT32_TEST_AND_SET_IS_SOMETIMES_NATIVE";
#endif

#if defined(Q_ATOMIC_INT32_TEST_AND_SET_IS_WAIT_FREE)
    qDebug() << "Q_ATOMIC_INT32_TEST_AND_SET_IS_WAIT_FREE";
#endif

#if defined(Q_ATOMIC_INT32_IS_SUPPORTED)
    qDebug() << "Q_ATOMIC_INT32_IS_SUPPORTED";
#endif
}

void test_atomic_features_runtime()
{
    qDebug() << "native *******************************************************";
    qDebug() << "fetch & add:" << QAtomicInteger<int>::isFetchAndAddNative();
    qDebug() << "fetch & store:" << QAtomicInteger<int>::isFetchAndStoreNative();
    qDebug() << "test & set:" << QAtomicInteger<int>::isTestAndSetNative();
    qDebug() << "reference counting:" << QAtomicInteger<int>::isReferenceCountingNative();

    qDebug() << "wait free ****************************************************";
    qDebug() << "fetch & add:" << QAtomicInteger<int>::isFetchAndAddWaitFree();
    qDebug() << "fetch & store:" << QAtomicInteger<int>::isFetchAndStoreWaitFree();
    qDebug() << "test & set:" << QAtomicInteger<int>::isTestAndSetWaitFree();
    qDebug() << "reference counting:" << QAtomicInteger<int>::isReferenceCountingWaitFree();
}

void test_atomic_function()
{
    {
        auto i = QAtomicInteger<int>{2};
        qDebug() << "i.ref():" << i.ref();
        qDebug() << "i.deref():" << i.deref();
        qDebug() << "i.deref():" << i.deref();
        qDebug() << "i.deref():" << i.deref();
    }
    {
        auto i = QAtomicInteger<int>{1};
        auto i2 = i.fetchAndAddAcquire(1);
        qDebug() << "original:" << i2 << ", new:" << i;
    }
    {
        auto i = QAtomicInteger<int>{10};
        qDebug() << "i.testAndSetAcquire(10):" << i.testAndSetAcquire(10, 5);
        qDebug() << "i:" << i;
    }
}

}

void test_atomic()
{
    //internal::test_atomic_features_using_macro();
    //internal::test_atomic_features_runtime();
    internal::test_atomic_function();
}
