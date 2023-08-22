#include <QFuture>
#include <QDebug>
#include <QList>
#include <QtConcurrent>

namespace { //=================================================================

namespace _1 {

auto x2 = [](int value) -> int {
    return value * value;
};

void test(QObject* parent)
{
    QList<int> lists{1, 2, 3, 4, 5};

    auto future = QtConcurrent::map(lists, x2);

    future.waitForFinished();

    qDebug() << lists;

    auto count = future.resultCount();
    qDebug() << "resultCount(): " << count;
    for (int i=0; i<count; i++) {
        //qDebug() << future.result(i);
    }

}


} //_1 --------------------------------------------------------------

} //===========================================================================

void test_future(QObject* parent)
{
    _1::test(parent);
}
