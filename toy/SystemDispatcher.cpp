#include "SystemDispatcher.h"
#include <QMetaType>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "Login.h"

/******************************************************************************
 * Private Class
 *
 */
class SystemDispatcherPrivate
{
public:
    QQmlEngine *mEngine;
    QQuickItem *mRoot;

    void registMetaTypes();
};

void SystemDispatcherPrivate::registMetaTypes()
{
    Login::registerMetaType();
}

/******************************************************************************
 * SystemDispatcher
 *
 */

SystemDispatcher::SystemDispatcher(QQmlEngine *engine, QObject *parent)
    : Dispatcher(parent)
    , d(new SystemDispatcherPrivate)
{
    d->mEngine = engine;
    d->mRoot = nullptr;

    d->registMetaTypes();
}

SystemDispatcher::~SystemDispatcher()
{
    delete d;
}

void SystemDispatcher::dispatch(int action, const QVariant &param)
{
    switch (action) {
    case RegisterRoot:
        {
            auto m = param.toMap();
            auto root = m["root"].value<QQuickItem*>();
            d->mRoot = root;
        }
        break;
    case CreateView:
        {
            auto m = param.toMap();
            auto viewName = m["viewName"].toString();
            auto args = m["args"].toMap();
#ifdef QT_DEBUG
            qDebug() << "viewName: " << viewName;

            for (auto key: args.keys()) {
                qDebug() << "args." << key << ":" << args[key];
            }
#endif

            View *view = nullptr;

            auto type = QMetaType::fromName(viewName.toStdString().c_str());
            auto id = type.id();
            if (id != QMetaType::UnknownType) {
                auto *inst = type.create(id);
                view = reinterpret_cast<View*>(inst);
            }

            if (view == nullptr) {
                qDebug() << "Can't create view: " << viewName;
                return;
            }

            auto qmlPath = QString{"qrc:/%1.qml"}.arg(viewName);

            auto initialProperties = QVariantMap{
                {"view", QVariant::fromValue(view)}
            };
            auto *c = new QQmlComponent(d->mEngine, QUrl{qmlPath});
#if (0)
            auto *item = qobject_cast<QQuickItem*>(c->createWithInitialProperties(initialProperties));
#else
            auto *rootContext = d->mEngine->rootContext();

            // #1. build object hierarchy, constant value assigned
            auto *item = qobject_cast<QQuickItem*>(c->beginCreate(rootContext));
            if (item != nullptr) {
                item->setParentItem(d->mRoot);
            } else {
                qDebug() << "load fail:" << qmlPath;
                qDebug() << c->errorString();
            }

            // #2. for setting qml properties
            view->setQml(item);

            // #3. set cpp view to qml view.
            c->setInitialProperties(item, initialProperties);

            // #4. property bindings. componentCompleted
            c->completeCreate();

            view->subscribe(Created);
#endif
        }
        break;
    }
}
