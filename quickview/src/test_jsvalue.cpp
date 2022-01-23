#include <QObject>
#include <QJSEngine>
#include <QJSValue>
#include <qqmlregistration.h>
#include <QHash>
#include <QRandomGenerator>

class Cache : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    Q_INVOKABLE QJSValue lookup(const QString &key) {
        if (auto it = mCache.constFind(key); it != mCache.constEnd()) {
            return *it;
        } else {
            return QJSValue::UndefinedValue;
        }
    }

private:
    QHash<QString, QString> mCache;
};

namespace internal
{

void testJsApi()
{
    auto engine = QJSEngine{};
    auto obj = engine.newObject();
    obj.setProperty("num", 42);
    auto function = engine.evaluate("(o) => o.num *= 2");
    auto args = QJSValueList { obj };
    auto result = function.call(args);

    qDebug() << "42 * 2 = " << result.toInt();
}

void testJsException()
{
    auto engine = QJSEngine{};
    auto val = engine.evaluate("let o = { get f() { throw 42; } }; o");
    val.property("f");

    qDebug() << "hasError: " << engine.hasError();

    val = engine.evaluate("let o = { get f() { throw 42; } }; o");
    auto managed = QJSManagedValue{ std::move(val), &engine };
    managed.property("f");

    qDebug() << "managed hasError: " << engine.hasError();

    auto error = engine.catchError();

    qDebug() << "error: " << error.toInt();
}

void testJsArray()
{
    auto engine = QJSEngine{};
    auto array = engine.newArray(5);
    for (auto i=0; i<5; i++) {
        array.setProperty(i, QRandomGenerator::global()->generate());
    }

    qDebug() << "isArray ? " << array.isArray();

    auto length = array.property("length").toInt();
    qDebug() << "array length: " << length;


    for (auto i=0; i<length; i++) {
        qDebug() << "array[" << i << "]: " << array.property(i).toInt();
    }
}

void testJsValueOperator()
{
    auto v0 = QJSValue{9};
    auto v1 = QJSValue{"9"};

    qDebug() << "v0.equals(v1) ? " << v0.equals(v1);
    qDebug() << "v0.strictlyEquals(v1) ? " << v0.strictlyEquals(v1);
}

}

void testJsValue()
{
    //internal::testJsApi();
    //internal::testJsException();
    internal::testJsArray();
    internal::testJsValueOperator();
}

#include "test_jsvalue.moc"
