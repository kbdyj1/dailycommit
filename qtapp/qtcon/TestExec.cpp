#include "TestExec.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <openssl/aes.h>
#include <openssl/evp.h>

#define COMPRESSING

#ifndef COMPRESSING
#define OUTPUT_FILENAME "tmpinfo.txt"
#else
#define ENCRYPTION
#define OUTPUT_FILENAME "tmpinfo.zip"
//#define COMPRESSING_VERIFY
#endif

QByteArray encryption(QFile& file)
{
    const int MaxBufferSize = 1024;
    char readBuffer[MaxBufferSize];
    auto* ctx = EVP_CIPHER_CTX_new();
    int enc = 1;
    uchar KEY_CRYPT[] = "__gnu_cxx17__normal_iteratorIPKcS4_EEmc";
    uchar KEY_IV[] = "_ZNK10QStringRef8containsE5QCharN2Qt15CaseSensitivityE";
    EVP_CipherInit(ctx, EVP_aes_128_cbc(), KEY_CRYPT, KEY_IV, enc);
    auto blockSize = EVP_CIPHER_CTX_block_size(ctx);
    auto cipherBuffer = QByteArray(MaxBufferSize + blockSize, Qt::Uninitialized);

    int outLen;
    auto out = QByteArray();
    while (!file.atEnd()) {
        auto len = file.read(readBuffer, MaxBufferSize);
        EVP_CipherUpdate(ctx, (uchar*)cipherBuffer.data(), &outLen, (uchar*)readBuffer, len);
        out.append(cipherBuffer.data(), outLen);
    }
    EVP_CipherFinal(ctx, (uchar*)cipherBuffer.data(), &outLen);
    out.append(cipherBuffer.data(), outLen);

    EVP_CIPHER_CTX_free(ctx);

    return out;
}

TestExec::TestExec(QObject* parent) : QObject(parent)
{
    process = new QProcess(this);
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
}

TestExec::~TestExec()
{
}

void TestExec::start()
{
    auto program = "ls";
    auto option = QStringList{};
    option << "-S" << "-R" << "-s" << "-h" << "/tmp/";

    qDebug() << "process->start(" << program << option << ")";

    //process->setStandardOutputFile("tmplist.txt");
    process->start(program, option);
}

void TestExec::onFinished(int exitCode, QProcess::ExitStatus status)
{
    if (0 == exitCode) {
        auto output = process->readAll();
#ifdef COMPRESSING
                                            // 3270 bytes (original size)
        const int CompressionLevelNo  = 0;  // 3285 bytes (no compressing)
        const int CompressionLevelMin = 1;  //  672 bytes
        const int CompressionLevelMax = 8;  //  638 bytes
        const int compressionLevel = CompressionLevelMax;

        output = qCompress(output, compressionLevel);
#endif
        auto file = QFile(OUTPUT_FILENAME);
        if (file.open(QFile::WriteOnly)) {
            file.write(output);
            file.close();
        }

#ifdef ENCRYPTION
        if (file.open(QFile::ReadOnly)) {
            auto out = encryption(file);
            file.close();

            file.setFileName("tempinfo.enc");
            if (file.open(QFile::WriteOnly)) {
                file.write(out);
                file.close();
            }
        }
#endif
    } else {
        qDebug() << "Process signal finished(" << exitCode << ", " << status << ")";
        qDebug() << process->errorString();
    }

#ifdef COMPRESSING
#ifdef COMPRESSING_VERIFY
    auto file = QFile(OUTPUT_FILENAME);
    if (file.open(QFile::ReadOnly)) {
        auto data = file.readAll();
        file.close();

        data = qUncompress(data);
        qDebug() << "Verifying...\n\n";
        qDebug() << data.constData();
        qDebug() << "\n\n======================================================================\n";
    }
#endif
#endif
}
