#include <QJSEngine>
#include <QObject>

class MyObject : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE MyObject() : QObject(nullptr)
    {
        qDebug() << "MyObject::MyObject()";
    }
    Q_INVOKABLE MyObject(int val) : QObject(nullptr)
    {
        qDebug() << "MyObject::MyObject(" << val << ")";
    }
    virtual ~MyObject()
    {
        qDebug() << "MyObject::~MyObject()";
    }
};

namespace internal
{

void testEval()
{
    auto engine = QJSEngine{};
    auto eval = QLatin1String{"1+2"};
    auto ret = engine.evaluate(eval);

    qDebug() << eval << " = " << ret.toInt();
}

void testFunction()
{
    auto engine = QJSEngine{};
    auto fun = engine.evaluate("(function(a,b){ return a+b; })");
    auto args = QJSValueList{};
    args << 1 << 2;
    auto ret = fun.call(args);

    qDebug() << "function(" << args.at(0).toInt() << "," << args.at(1).toInt() << ")=" << ret.toInt();
}

void testModule()
{
    auto engine = QJSEngine{};
#if (0)
    auto module = engine.importModule(":/res/math.mjs");
    auto fun = module.property("sum");
#else
    auto module = engine.importModule(":/res/twice.mjs");
    auto fun = module.property("addTwice");
#endif
    auto args = QJSValueList{};
    args << 1 << 2;
    auto ret = fun.call(args);

    qDebug() << "function(" << args.at(0).toInt() << "," << args.at(1).toInt() << ")=" << ret.toInt();
}

void testRegisterModule()
{
    auto engine = QJSEngine{};

    engine.globalObject().setProperty("lang", "ko");

    auto version = QJSValue{620};
    engine.registerModule("version", version);
    auto name = QJSValue{"Qt6"};
    auto qt6 = engine.newObject();
    qt6.setProperty("name", name);
    engine.registerModule("info", qt6);

    auto module = engine.importModule(":/res/version.mjs");

    if (!module.isError()) {
        auto fun = module.property("getVersion");
        auto ret = fun.call();

        qDebug() << "getVersion() :" << ret.toString();

        fun = module.property("getName");
        ret = fun.call();
        qDebug() << "getName() :" << ret.toString();

    } else {
        qDebug() << "Uncaught exception at line"
                 << module.property("lineNumber").toInt()
                 << ":"
                 << module.toString();

        // Uncaught exception at line 0 : "Error: Could not open module qrc:/res/name for reading"
    }
}

void testJsMetaObject()
{
    auto engine = QJSEngine{};
    auto mo = engine.newQMetaObject(&MyObject::staticMetaObject);

    engine.globalObject().setProperty("MyObject", mo);

    auto eval = QLatin1String{ "var o = new MyObject(5)" };
    qDebug() << "eavl(" << eval << ")";

    engine.evaluate(eval);
}

void testJsDynamicProperties()
{
    auto engine = QJSEngine{};
    auto *object = new QObject{};
    object->setProperty("dynamicValue", 5);

    // dynamic QObject properties are not supported !!!

    auto jsObject = engine.newQObject(object);

    engine.globalObject().setProperty("myObject", jsObject);

    auto eval = QLatin1String{"myObject.dynamicValue"};
    auto ret = engine.evaluate(eval);
    if (ret.isError()) {
        qDebug() << eval << ":" << ret.toString();
    } else {
        qDebug() << eval << ": " << ret.toInt(); // "myObject.dynamicValue" :  0
    }
}

void testJsEngineExtension()
{
    auto engine = QJSEngine{};
#if (0)
    engine.installExtensions(QJSEngine::ConsoleExtension); // js: Qt 6 QJSEngine example
#endif
    auto eval = QLatin1String{"console.log('Qt 6 QJSEngine example')"};
    engine.evaluate(eval);
}

} //=========================================================================== internal

void testJsEngine()
{
    //internal::testEval();
    //internal::testFunction();
    //internal::testModule();
    //internal::testRegisterModule();
    //internal::testJsMetaObject();
    //internal::testJsDynamicProperties();
    internal::testJsEngineExtension();
}

#include "test_jsengine.moc"
