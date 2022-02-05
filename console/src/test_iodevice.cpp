#include <QIODevice>
#include <QBuffer>
#include <QDebug>

namespace internal
{

void test_buffer()
{
#if (1)
    auto array = QByteArray{};
    auto buffer = QBuffer{&array};
#else
    auto buffer = QBuffer{};
#endif

    char c;
    buffer.open(QBuffer::ReadWrite);
    buffer.write("QBuffer test");
    buffer.seek(0);

    auto equalCount = 0;
    buffer.getChar(&c);
    if (c == 'Q')
        equalCount++;
    buffer.getChar(&c);
    if (c == 'B')
        equalCount++;

    qDebug() << "equal: " << equalCount;
}

}

void test_iodevice()
{
    internal::test_buffer();
}
