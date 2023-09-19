#include <QPointer>
#include <QObject>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

class A : public QObject
{
public:
    A() {
        qDebug() << "A()";
    }
    virtual ~A() {
        qDebug() << "~A()";
    }
    void print() {
        qDebug() << "A::print()";
    }
    void destroy() {
        deleteLater();
    }
};

void func(A* a)
{
    a->print();
    a->destroy();
}

void test()
{
    QPointer<A> a = new A;

    func(a);

    qDebug() << "end of test()";
}

} //_1 --------------------------------------------------------------

} //===========================================================================


void test_qpointer()
{
    _1::test();
}
