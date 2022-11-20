#include <QtCborCommon>
#include <QCborValue>
#include <QCborStreamWriter>
#include <QFile>
#include <QMap>
#include <QBitArray>
#include <QDateTime>

namespace { //=================================================================

namespace _1 {

void writeFloat(QCborStreamWriter& writer, float f)
{
    qfloat16 f16 = f;
    if (qIsNaN(f) || f16 == f)
        writer.append(f16);
    else
        writer.append(f);
}

void writeDouble(QCborStreamWriter& writer, double d)
{
    float f;
    if (qIsNaN(d)) {
        writer.append(qfloat16(qQNaN()));
    } else if (qIsInf(d)) {
        writer.append(d < 0 ? -qInf() : qInf());
    } else if ((f = d) == d) {
        qfloat16 f16 = f;
        if (f16 == f)
            writer.append(f16);
        else
            writer.append(f);
    } else {
        writer.append(d);
    }
}

#define OCCUR_MAP_ERROR

void writeTest(QCborStreamWriter& writer)
{
    writer.append("Hello, Qt");
    writer.append(100);

    writer.startMap(3);
    {
        writer.append(0);
        writer.append("Zero");

        writer.append(1);
        writer.append("One");

        writer.append("color");
        writer.startArray(3);
        {
            writer.append("Red");
            writer.append("Green");
            writer.append("Blue");
        }
        writer.endArray();

#if defined(OCCUR_MAP_ERROR)
        writer.append("invalid key");   //QCborStreamWriter: too many items added to array or map
#endif
    }
    writer.endMap();
}

void test(QCborStreamWriter& writer)
{
    writeTest(writer);
    writeFloat(writer, 1.5f);
}

} //_1 --------------------------------------------------------------

namespace _2 { //NAN, INF

void test()
{
    auto inf = qInf();
    auto ninf = -qInf();

    auto nan = qQNaN();
    auto b0 = QBitArray::fromBits((const char*)&inf, sizeof(double)*8);
    auto b1 = QBitArray::fromBits((const char*)&ninf, sizeof(double)*8);
    auto b2 = QBitArray::fromBits((const char*)&nan, sizeof(double)*8);


    qDebug() << "inf : " << b0;
    qDebug() << "-inf: " << b1;
    qDebug() << "SNAN: " << b2;

    qDebug() << "SNAN == SNAN: " << (SNAN == SNAN);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test(QCborStreamWriter& writer)
{
    auto dt = QDateTime::currentDateTime();
    auto text = dt.toString(Qt::ISODate);
    qDebug() << text;

    writer.append(QCborKnownTags::DateTimeString);
    writer.append(text);

    auto url = QUrl("https://doc.qt.io/qt-6/qcborstreamwriter.html#append-6");
    text = url.toString();
    qDebug() << "url: " << text;
    writer.append(QCborKnownTags::Url);
    writer.append(text);
}

} //_3 --------------------------------------------------------------

} //===========================================================================

void test_cbor()
{
    auto bytes = QByteArray();
    auto writer = QCborStreamWriter(&bytes);
#if (0) //done
    _1::test(writer);
    _2::test();
#endif

    _3::test(writer);

    qDebug() << "CBOR stream.";
    qDebug() << bytes;
}
