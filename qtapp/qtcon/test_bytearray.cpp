#include <QByteArray>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test()
{
    auto encoded = QByteArray("Qt%20is%20great%33");
    auto decoded = encoded.fromPercentEncoding(encoded);

    qDebug() << decoded;
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_bytearray()
{
    _1::test();
}
