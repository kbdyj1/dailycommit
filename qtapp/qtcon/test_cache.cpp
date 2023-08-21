#include <QCache>
#include <QDebug>
#include <QString>
#include <QByteArray>

namespace {  //================================================================

namespace _1 {

struct Employee
{
    Employee(int id, const QString& name) : id(id), name(name)
    {}
    ~Employee()
    {
        qDebug() << "~Employee(" << id << ", " << name << ")";
    }

    int id;
    QString name;
};

QCache<QString,Employee> cache{3};

void test()
{
    cache.insert("a", new Employee{1, "aaa"});
    cache.insert("b", new Employee{2, "bbb"});
    cache.insert("c", new Employee{3, "ccc"});

    qDebug() << "access a: " << cache.object("a")->name;

    cache.insert("d", new Employee{4, "ddd"});

    qDebug() << "access c: " << cache.object("c")->name;

    cache.insert("b", new Employee{5, "eee"});

    cache.take("d");

    qDebug() << "cache.totalCost(): " << cache.totalCost();
    qDebug() << "cache.maxCost(): " << cache.maxCost();
    qDebug() << "cache.count(): " << cache.count();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_cache()
{
    _1::test();
}
