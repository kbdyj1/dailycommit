#include <QVarLengthArray>
#include <QElapsedTimer>
#include <QDebug>
#include <QVector>

namespace internal
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

} //internal

void testQtContinaer()
{
    internal::testVarLengthArray();
}
