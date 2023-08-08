#include <QByteArray>
#include <QDebug>
#include <QFile>

namespace { //=================================================================

namespace _1 {

void test()
{
    auto encoded = QByteArray("Qt%20is%20great%33");
    auto decoded = encoded.fromPercentEncoding(encoded);
    qDebug() << decoded;

    const char tag[] = {'c', 'a', '\0', 'r', '\0', 't'};

    auto nonDeepCopy = QByteArray::fromRawData(tag, 6);
    qDebug() << nonDeepCopy << ", qstrlen(): " << qstrlen(nonDeepCopy);

    const void* p0 = nonDeepCopy.constData();
    qDebug() << p0;

    nonDeepCopy.resize(32);
    qDebug() << nonDeepCopy << ", qstrlen(): " << qstrlen(nonDeepCopy);

    const void* p1 = nonDeepCopy.constData();
    qDebug() << p1;

    QByteArray ba("We must be <b>bold</b>, very <b>bold</b>");
    qDebug() << "count(<b>): " << ba.count("<b>");
    qDebug() << "contains(<b>): " << ba.contains("<b>");

    auto null = QByteArray{};

    qDebug() << "null.data(): " << (void*)null.data();
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(const char* filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        auto data = file.readAll();
        qDebug() << data;

        auto trimmed = data.trimmed();
        qDebug() << "trimmed -------------------------------\n" << trimmed;

        auto simplified = data.simplified();
        qDebug() << "simplified ----------------------------\n" << simplified;
    }
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_bytearray(int argc, char* argv[])
{
#if (0)
    _1::test();
#endif

    if (1 < argc) {
        _2::test(argv[1]);
    }
}
