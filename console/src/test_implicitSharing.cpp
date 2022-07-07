#include <QSharedData>
#include <QDebug>

namespace { //-----------------------------------------------------------------

class EmployeeData : public QSharedData
{
public:
    int mId = -1;
    QString mName;

    EmployeeData()
    {}
    EmployeeData(const EmployeeData& o)
        : QSharedData(o)
        , mId(o.mId)
        , mName(o.mName)
    {}
    virtual ~EmployeeData()
    {}
};

class Employee
{
    QSharedDataPointer<EmployeeData> d;
public:
    Employee()
        : d(new EmployeeData)
    {}
    Employee(int id, const QString& name)
        : d(new EmployeeData)
    {
        setId(id);
        setName(name);
    }
    Employee(const Employee& o)
        : d(o.d)
    {}
    void setId(int id)
    {
        d->mId = id;
    }
    void setName(const QString& name)
    {
        d->mName = name;
    }
    int id() const
    {
        return d->mId;
    }
    QString name() const
    {
        return d->mName;
    }
    void print()
    {
        qDebug() << "ref : " << d->ref.ref();
    }
};

void test(Employee e)
{
    e.print();
    qDebug() << "e.name() : " << e.name();
    e.print();
    qDebug() << "e.id() : " << e.id();

    qDebug() << "e.setId(2)";
    e.setId(2);
    e.print();

    auto ec = e;
    ec.print();
}

} // namespace ----------------------------------------------------------------

void test_implicitSharing()
{
    auto e = Employee { 1, "unnamed" };
    e.print();

    qDebug() << "call test(e)";
    test(e);
}
