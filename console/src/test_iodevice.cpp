#include <QIODevice>
#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QMap>
#include <QVariant>
#include <QDir>

namespace internal
{

void test_qbuffer()
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

#define WRITE_QSTRING

void test_serialize()
{
    auto filename = QLatin1String{"test.dat"};
    auto file = QFile{filename};

    auto text = QLatin1String{"Hello, Qt6 !!!"};
    auto map = QMap<QString, QVariant>{
        {"0", "Hello"},
        {"1", "Qt"},
        {"2", 6}
    };

    qDebug() << "#1 write";

    if (file.open(QFile::WriteOnly)) {
        auto out = QDataStream{&file};

        out << QDataStream::Qt_6_2;

#ifdef WRITE_QSTRING
        auto str = QString{text};
#else
        // int32(len) + raw data(... + '\0')
        auto *str = text.data();
#endif
        out << str;
        out << map;
        file.close();
    }

    qDebug() << "#1 read";

    if (file.open(QFile::ReadOnly)) {
        auto in = QDataStream{&file};
        qint32 version;
        in >> version;

        qDebug() << "version:" << version;
        in.setVersion(version);

#ifdef WRITE_QSTRING
        auto str = QString{};
        in >> str;
        qDebug() << str;

        //0000000 0000 1c00 4800 6500 6c00 6c00 6f00 2c00
        //0000010 2000 5100 7400 3600 2000 2100 2100 2100
#else
        qint32 len;
        in >> len;
        char buffer[256];
        auto result = in.readRawData(buffer, len);

        //0000000 0000 0f00 6548 6c6c 2c6f 5120 3674 2120
        //0000010 2121 0000

        qDebug().nospace() << "result: " << result << ", len: " << len << ", data: " << buffer;
#endif
        auto map = QMap<QString, QVariant>{};
        in >> map;

        //QMap(("0", QVariant(QString, "Hello"))("1", QVariant(QString, "Qt"))("2", QVariant(int, 6)))
        qDebug() << map;

        file.close();
    }
}

void test_qdir()
{
    qDebug() << "home:" << QDir::homePath();    //home: "/home/xxx"
    qDebug() << "root:" << QDir::rootPath();    //root: "/"
    qDebug() << "temp:" << QDir::tempPath();    //temp: "/tmp"
    qDebug() << "separator:" << QDir::listSeparator();  //separator: ':'

    auto homePath = QDir::homePath();

    auto dir = QDir::home();

    qDebug() << "dir.path():" << dir.path();     //dir.path(): "/home/xxx"
    qDebug() << "dir.isAbsolute():" << dir.isAbsolute();    //dir.isAbsolute(): true
    qDebug() << "dir.dirName():" << dir.dirName();  //dir.dirName(): "xxx"
}

}

void test_iodevice()
{
//    internal::test_qbuffer();
//    internal::test_serialize();
    internal::test_qdir();
}
