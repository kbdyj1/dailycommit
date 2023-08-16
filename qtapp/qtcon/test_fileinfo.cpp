#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

namespace { //=================================================================

namespace _1 {

void test(const char* filename)
{
#if (1)
    auto fi = QFileInfo{filename};
#else
    auto fi = QFileInfo{"/tmp/archive.tar.gz"};
#endif

    fi.refresh();

    qDebug() << "absolutePath: " << fi.absolutePath();
    qDebug() << "baseName: " << fi.baseName();
    qDebug() << "fileName: " << fi.fileName();
    qDebug() << "filePath: " << fi.filePath();
    qDebug() << "birthTime: " << fi.birthTime().toString();
    qDebug() << "bundleName: " << fi.bundleName();
    qDebug() << "canonicalFilePath: " << fi.canonicalFilePath();
    qDebug() << "canonicalPath: " << fi.canonicalPath();
    qDebug() << "completeBaseName: " << fi.completeBaseName();
    qDebug() << "FileBirthTime: " << fi.fileTime(QFile::FileBirthTime).toString();
    qDebug() << "FileAccessTime: " << fi.fileTime(QFile::FileAccessTime).toString();
    qDebug() << "FileMetadataChangeTime: " << fi.fileTime(QFile::FileMetadataChangeTime).toString();
    qDebug() << "FileModificationTime: " << fi.fileTime(QFile::FileModificationTime).toString();
    qDebug() << "group: " << fi.group() << ", groupId: " << fi.groupId();
    qDebug() << "owner: " << fi.owner() << ", ownerId: " << fi.ownerId();
    qDebug() << "isAbsolute: " << fi.isAbsolute();
    qDebug() << "isBundle: " << fi.isBundle();
    qDebug().nospace() << (fi.isDir() ? 'd' : '-') << (fi.isReadable() ? 'r' : '-') << (fi.isWritable() ? 'w' : '-') << (fi.isExecutable() ? 'x' : '-');
    qDebug() << "lastModified: " << fi.lastModified();
    qDebug() << "lastRead: " << fi.lastRead();
    qDebug() << "permissions: " << fi.permissions();
    qDebug() << "caching: " << fi.caching();
    qDebug() << "size: " << fi.size() << " bytes";
    qDebug() << "suffix: " << fi.suffix();
    qDebug() << "symLinkTarget: " << fi.symLinkTarget();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_fileinfo(const char* filename)
{
    _1::test(filename);
}
