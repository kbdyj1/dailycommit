#include <QQuickView>
#include <QQuickItem>
#include <QQmlEngine>

#include <sys/sysinfo.h>
#include <unistd.h>

// QML provides a JavaScript host environment (ECMA-262 7th edition)

void testJavaScript(QQuickView *view)
{
    // for testing Qt.exit(0)
    QObject::connect(view->engine(), &QQmlEngine::exit, qApp, &QCoreApplication::exit);

    view->setSource(QUrl("qrc:/quickview/qml/MyButton.qml"));
#if (0)
    auto processorsCnt = sysconf(_SC_NPROCESSORS_CONF);// get_nprocs();
    auto processorsConf = sysconf(_SC_NPROCESSORS_ONLN);// get_nprocs_conf();

    qDebug() << "processors conf:" << processorsConf << ", cnt:" << processorsCnt;
#endif
}
