#include <QFile>
#include <QDebug>
#include <QDateTime>

namespace { //=================================================================

namespace _1 {

void test(int argc, char *argv[])
{
    QString filename;
    if (1 < argc) {
        filename = argv[1];
    } else {
        filename = "/proc/zoneinfo";
    }
    auto fd = QFile{filename};
    if (fd.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "handle: " << fd.handle();
        qDebug() << "atEnd? " << fd.atEnd();
        qDebug() << "isSequential? " << fd.isSequential();
        qDebug() << "permissions: " << fd.permissions();
        qDebug() << "pos: " << fd.pos();
        qDebug() << "FileBirthTime: " << fd.fileTime(QFileDevice::FileBirthTime);
        qDebug() << "FileAccessTime: " << fd.fileTime(QFileDevice::FileAccessTime);

        auto* map = fd.map(0, fd.size());
        for (auto i=0; i<fd.size(); i++) {
            //qDebug() << "map[" << i << "]: " << (char)map[i];
            if (9 == i)
                break;
        }
        fd.unmap(map);
    }
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_file(int argc, char *argv[])
{
    _1::test(argc, argv);
}
