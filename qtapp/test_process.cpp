#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

namespace  { //================================================================

namespace _1 {

QProcess* process = nullptr;

auto onFinished = [](int exitCode, QProcess::ExitStatus status){
    qDebug() << "onFinished(" << exitCode << ", " << status << ")";

    if (process) {
        process->deleteLater();
        process = nullptr;
    }
};

auto onReadStdout = []() {
    if (process) {
        auto bytes = process->readAllStandardOutput();
        qDebug() << "stdout: " << bytes;
    }
};

auto onReadStderr = []() {
    if (process) {
        auto bytes = process->readAllStandardError();
        qDebug() << "stderr: " << bytes;
    }
};

void test(QObject* parent)
{
    auto program = QString{"ls"};
    auto args = QStringList{};
    process = new QProcess{parent};
    process->start(program, args);
#if (1)
    QObject::connect(process, &QProcess::finished, onFinished);
    QObject::connect(process, &QProcess::readyReadStandardOutput, onReadStdout);
    QObject::connect(process, &QProcess::readyReadStandardError, onReadStderr);
#else
    process->waitForFinished();
    qDebug() << "[Result]\n" << process->readAll();

    process->deleteLater();
#endif
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_process(QObject* parent)
{
    _1::test(parent);
}
