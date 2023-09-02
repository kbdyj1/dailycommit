#include <QMetaObject>
#include <QMetaClassInfo>
#include <QMetaMethod>
#include <QObject>
#include <QDebug>
#include <QScopedPointer>
#include <QString>

#ifndef Q_MOC_RUN
#   define IDTC_TAG
#endif

namespace { //=================================================================

class A : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("repository", "https://github.com/kbdyj1")
    Q_CLASSINFO("language", "C++/Qt")

public:
    Q_INVOKABLE A(QObject* parent = nullptr) : QObject(parent)
    {}
    Q_INVOKABLE A(const QString& name, QObject* parent) : QObject(parent)
    {}

private Q_SLOTS:
    IDTC_TAG void onMicomMessageReceived(const QString& message)
    {
        qDebug().nospace().noquote() << "onMicomMessageReceived(" << message << ")";
    }
};

const char* accessString(QMetaMethod::Access access)
{
    switch (access) {
    case QMetaMethod::Private: return "private";
    case QMetaMethod::Protected: return "protected";
    case QMetaMethod::Public: return "public";
    }
    return "invalid";
}

QString paramString(const QMetaMethod& mm)
{
    auto paramCnt = mm.parameterCount();
    auto types = mm.parameterTypes();
    auto names = mm.parameterNames();
    auto paramString = QString{};
    for (auto j=0; j<paramCnt; j++) {
        if (j)
            paramString += ", ";
        auto type = types.at(j);
        auto name = names.at(j);
        paramString += type;
        paramString += " ";
        paramString += name;
    }
    return paramString;
}

namespace _1 {

void test()
{
    auto a = QScopedPointer<A>{new A{}};
    auto* mo = a->metaObject();
    auto ciCnt = mo->classInfoCount();

    qDebug().nospace() << "classInfo count: " << ciCnt << ", offset: " << mo->classInfoOffset();
    for (auto i=0; i<ciCnt; i++) {
        auto ci = mo->classInfo(i);
        qDebug().nospace() << "\tclassInfo[" << i << "] " << ci.name() << " : " << ci.value();
    }

    auto constructCnt = mo->constructorCount();
    for (auto i=0; i<constructCnt; i++) {
        auto mm = mo->constructor(i);
        auto param = paramString(mm);

        qDebug().nospace().noquote() << accessString(mm.access()) << " " << mm.name() << "(" << param<< ")";
    }
    {
        auto funcIndex = mo->indexOfSlot("onMicomMessageReceived(QString)");
        auto mm = mo->method(funcIndex);
        auto param = paramString(mm);
        qDebug().nospace().noquote() << mm.tag() << " " << mm.typeName() << " " << mm.name() << "(" << param << ") ";
    }

    QMetaObject::invokeMethod(a.data(), "onMicomMessageReceived", Qt::DirectConnection, Q_ARG(QString, "Hello, Qt"));
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_metaobject()
{
    _1::test();
}

#include "test_metaobject.moc"
