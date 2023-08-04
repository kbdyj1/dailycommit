#include <QBuffer>
#include <QDebug>
#include <QTextStream>
#include <QScopedPointer>

class BufferMonitor : public QObject
{
    Q_OBJECT

public:
    BufferMonitor(QObject* parent = nullptr) : QObject(parent)
    {}
    virtual ~BufferMonitor()
    {}

public Q_SLOTS:
    void onWritten(qint64 bytes) {
        qDebug() << "written " << bytes << " bytes.";
    }
};

namespace { //=================================================================

namespace _1 {

void test()
{
    auto ba = QByteArray{};
    auto buffer = QBuffer(&ba);
    auto monitor = QScopedPointer<BufferMonitor>(new BufferMonitor);

    QObject::connect(&buffer, &QBuffer::bytesWritten, monitor.data(), &BufferMonitor::onWritten);

#if (0)
    if (buffer.open(QBuffer::WriteOnly)) {
        buffer.write("Hello, Qt6");
        buffer.seek(0);
        buffer.write("HELLO");
        buffer.close();
    }
#else
    if (buffer.open(QBuffer::WriteOnly)) {
        auto ts = QTextStream{&buffer};
        ts << "Hello,";
        ts << " Qt6";
    }
#endif

    qDebug() << "buffer.size(): " << buffer.size();
    qDebug() << ba;
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_buffer()
{
    _1::test();
}

#include "test_buffer.moc"
