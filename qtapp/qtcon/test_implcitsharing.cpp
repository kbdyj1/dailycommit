#include <QVector>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    QVector<int> a, b;
    QVector<int> c;

    a.resize(10000);
    a[0] = 5;

    QVector<int>::iterator i = a.begin();

    b = a;

    a[0] = 10;

    b.clear();
    QVector<int>().swap(b);

    int j = *i;

    qDebug() << "j: " << j;
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_implicitsharing()
{
    _1::test();
}
