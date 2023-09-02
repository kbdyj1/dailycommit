#include <QList>
#include <QString>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

QList<QString> pathList = { "aaa", "bbb", "ccc", "ddd", "eee" };

void test()
{
    int removed = 0;
    for (int i=0; i<pathList.count(); ) {
        if (pathList[i] == "eee") {
            qDebug() << "[removed] : " << pathList[i];
            pathList.removeAt(i);
            removed++;
        } else {
            qDebug() << "pathList[" << i << "] : " << pathList[i];
            i++;
        }
    }
    qDebug() << "removed: " << removed;
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_list()
{
    _1::test();
}
