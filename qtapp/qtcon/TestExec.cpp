#include "TestExec.h"
#include <QDebug>

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

    process->setStandardOutputFile("tmplist.txt");
    process->start(program, option);
}

void TestExec::onFinished(int exitCode, QProcess::ExitStatus status)
{
    if (0 == exitCode) {
        auto output = process->readAll();
        qDebug() << "process->readAll() ->\n" << output;
    } else {
        qDebug() << "Process signal finished(" << exitCode << ", " << status << ")";
        qDebug() << process->errorString();
    }
}
