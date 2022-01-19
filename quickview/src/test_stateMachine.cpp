#include <QQuickView>
#include <QQuickItem>
#include <QDebug>
#include <QStateMachine>
#include <QState>
#include <QFinalState>

class Listener : public QObject
{
    Q_OBJECT

public:
    Listener(QObject *button, QObject *parent = nullptr)
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

public Q_SLOTS:
    void print() {
        qDebug() << "Listener";
    }
    void print(QString str) {
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

    qDebug() << "className: " << metaObject->className();
    qDebug() << "methodOffset: " << metaObject->methodOffset() << ", methodCount: " << metaObject->methodCount();

    for (auto i = metaObject->methodOffset(); i < metaObject->methodCount(); i++) {
        qDebug() << metaObject->method(i).methodSignature();
    }
}

#include "test_stateMachine.moc"
