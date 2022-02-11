#include <QXmlStreamReader>
#include <QBuffer>
#include <QFile>
#include <QDebug>

namespace internal
{

void displayXmlElements(QIODevice *io)
{
    auto reader = QXmlStreamReader{io};
    if (reader.readNextStartElement()) {
        auto name = reader.name();
        qDebug() << "name:" << name;

        while (reader.readNextStartElement()) {
            auto attribute = reader.name();
            auto value = reader.readElementText();
            auto col = reader.columnNumber();
            auto row = reader.lineNumber();

            qDebug() << "row:" << row << "col:" << col << "attribute:" << attribute << ", value:" << value;
        }
    }
}

void displayXmlWithReadNext(QIODevice *io)
{
    auto reader = QXmlStreamReader{io};
    while (reader.readNext() != QXmlStreamReader::EndDocument) {
        auto text = QStringView{};
        auto token = reader.tokenType();

        switch (token) {
        case QXmlStreamReader::StartDocument:
        case QXmlStreamReader::StartElement:
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::EndDocument:
            text = reader.name();
            break;
        default:
            text = reader.text();
            break;
        }

        qDebug() << reader.tokenString() << ", name:" << text;
    }
}

void test_qxmlstreamreader_file()
{
    auto file = QFile{":/note.xml"};
    auto result = file.open(QFile::ReadOnly);

    if (result) {
        //displayXmlElements(&file);
        displayXmlWithReadNext(&file);
    }
}

void test_qxmlstreamreader_string()
{
    auto str = QByteArray{
        "<string>\n"
            "<id>qt6</id>\n"
            "<company>qt comapnay</company>\n"
            "<age>10</age>\n"
            "<class>framework</class>\n"
        "</string>"
    };
    auto io = QBuffer{&str};
    auto result = io.open(QBuffer::ReadOnly);
    if (result) {
        displayXmlElements(&io);
    }
}

}

void test_xml()
{
    internal::test_qxmlstreamreader_file();
    //internal::test_qxmlstreamreader_string();
}
