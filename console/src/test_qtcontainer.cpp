#include <QVarLengthArray>
#include <QElapsedTimer>
#include <QDebug>
#include <QVector>
#include <QList>
#include <QString>

namespace internal //==========================================================
{

const int MAX_COUNT = 1024;

void func0(int len)
{
    // for supporting variable-length array

    QVarLengthArray<int, MAX_COUNT> array(len);
    for (auto iter=array.begin(); iter!=array.end(); iter++)
        *iter = 0;
}
void func1(int len)
{
    auto *data = new int[len];
    QVector<int> array(&data[0], &data[len-1]);
    for (auto iter=array.begin(); iter!=array.end(); iter++)
        *iter = 0;
    delete [] data;
}

void testVarLengthArray()
{
    QElapsedTimer elapsed;
    elapsed.start();
    for (auto i=1; i<MAX_COUNT; i++) {
#if (1)
        func0(i); // 1ms
#else
        func1(i); // 13~14ms
#endif
    }
    qDebug() << "QVarLengthArray performance : " << elapsed.elapsed() << "ms";
}

void testQList()
{
    QList<QString> list;

//    qDebug() << "QList<QString>.capacity(): " << list.capacity(); // 0

    list.append("one");
    list.append("two");
    auto three = QString{"three"};
    list.append(std::move(three));
    list.append("two");

//    list.emplace(1, 4, 'b');

    list.removeOne("two");
    //list.resize(8);

    qDebug() << list;
    qDebug() << three;

//    qDebug() << "QList<QString>.capacity(): " << list.capacity(); // 4

//    list.squeeze();
    list.swapItemsAt(0, 1);
//    qDebug() << "squized: " << list << ", capacity: " << list.capacity();


}

void testQListImplicitSharing()
{
    QList<int> a, b;
    a.reserve(100000);

    auto i = a.begin();

    b = a;

    a[0] = 5;   // a is detached from the shared data.
                // i is now b's iterator

    b.clear();  // < Qt5.7 : release the allocated memory

    auto j = *i;//undefined behavior !!!

    qDebug() << j;
}

} //internal ==================================================================

void testQtContinaer()
{
    //internal::testVarLengthArray();
    internal::testQList();
    //internal::testQListImplicitSharing();
}
