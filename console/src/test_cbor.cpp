#include <QtCborCommon>
#include <QCborValue>
#include <QCborStreamWriter>
#include <QFile>
#include <QMap>

namespace { //=================================================================

} //===========================================================================

void test_cbor()
{
    auto file = QFile("test.cbor");
    if (file.open(QFile::WriteOnly)) {
        auto writer = QCborStreamWriter(&file);
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
        }
        writer.endMap();
    }
}
