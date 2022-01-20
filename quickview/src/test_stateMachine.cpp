#include <QQuickView>
#include <QQuickItem>
#include <QDebug>
#include <QStateMachine>
#include <QState>
#include <QFinalState>

#ifndef Q_MOC_RUN
#   define SLOT
#   define INVOKE
#endif

class Listener : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("author", "yeongmin.hong")
    Q_CLASSINFO("url", "github.com/kbdyj1")

public:
    enum Type {
        TYPE0,
        TYPE1,
        TYPE2,
        TYPE_MAX
    };
    Q_ENUM(Type)

    Q_INVOKABLE Listener(QObject *button, QObject *parent = nullptr)
        : QObject(parent)
    {
        auto *main = new QState();

            auto *s1 = new QState(main);
            auto *s2 = new QState(main);
            auto *s3 = new QState(main);

            s1->addTransition(button, SIGNAL(clicked()), s2);
            s2->addTransition(button, SIGNAL(clicked()), s3);
            s3->addTransition(button, SIGNAL(clicked()), s1);
            s1->assignProperty(button, "color", "red");
            s2->assignProperty(button, "color", "green");
            s3->assignProperty(button, "color", "blue");

        main->setInitialState(s1);

        auto *exit = new QFinalState();
        main->addTransition(button, SIGNAL(exited()), exit);

        machine.addState(main);
        machine.addState(exit);
        machine.setInitialState(main);
        machine.start();

        connect(&machine, &QStateMachine::finished, this, [this](){
            qDebug() << "state machine finished !!!";

            // reset
            machine.start();
        });
    }

    Q_INVOKABLE INVOKE void invokeMethod(int value, QString string) {
        qDebug() << "invokeMethod(" << value << ", " << string << ")";
    }

public Q_SLOTS:
    SLOT void print() {
        qDebug() << "Listener";
    }
    SLOT void print(QString str) {
        qDebug() << "Listener: " << str;
    }

private:
    QStateMachine machine;
};

void testStateMachine(QQuickView *view)
{
    view->setSource(QUrl("qrc:/quickview/qml/TestStateMachine.qml"));
    auto *rootObject = view->rootObject();

    auto *listener = new Listener(rootObject, view);
    const auto *metaObject = listener->metaObject();
    const auto ci0 = metaObject->classInfo(0);
    const auto ci1 = metaObject->classInfo(1);

    qDebug() << "className: " << metaObject->className();
    qDebug() << "methodOffset: " << metaObject->methodOffset() << ", methodCount: " << metaObject->methodCount();
    qDebug() << "class info(0) " << ci0.name() << ": " << ci0.value();
    qDebug() << "class info(1) " << ci1.name() << ": " << ci1.value();

    for (auto i = metaObject->methodOffset(); i < metaObject->methodCount(); i++) {
        qDebug() << metaObject->method(i).methodSignature() << ", tag: " << metaObject->method(i).tag();
    }

    qDebug() << "constructor ==============================================================";
    auto constructorCount = metaObject->constructorCount();
    qDebug() << "constructor count: " << constructorCount;
    if (constructorCount) {
        for (auto i=0; i<constructorCount; i++) {
            auto method = metaObject->constructor(i);
            qDebug() << "method index: " << method.methodIndex();
            qDebug() << "method signature: " << method.methodSignature();
            qDebug() << "name: " << method.name();
            qDebug() << "parameter count: " << method.parameterCount();
            qDebug() << "\n";
        }
    }

    auto *obj = metaObject->newInstance(Q_ARG(QObject*, rootObject), Q_ARG(QObject*, view));

    QScopedPointer<QObject> p{ obj };

    if (!p.isNull()) {
        auto * l = dynamic_cast<Listener*>(p.data());
        if (l) {
            l->print(QLatin1String("Hello Qt !!!"));
        }
    }

    auto metaType = metaObject->metaType();
    auto id = metaType.fromName("Listener").id();
    qDebug() << "meta type id: " << id;

    if (metaObject->enumeratorCount()) {
        auto enum0 = metaObject->enumerator(0);
        qDebug() << "enum name: " << enum0.enumName();
        for (auto i=0; i<enum0.keyCount(); i++) {
            qDebug() << enum0.key(i) << ": " << enum0.value(i);
        }
    }
}

#include "test_stateMachine.moc"
