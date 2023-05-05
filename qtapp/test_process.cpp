#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QFile>

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

namespace _2 {

void test(QObject* parent)
{
    auto program = QString{"gzip"};
    auto* process = new QProcess(parent);

    process->start(program, QStringList() << "-c");
    if (!process->waitForStarted()) {
        qDebug() << program << " run failed.";
        return;
    }

    process->write("Hello, Qt6!!!");
    process->closeWriteChannel();

    if (!process->waitForFinished()) {
        qDebug() << "waitForFinished() error.\n";
        return;
    }

    auto result = process->readAll();
    qDebug() << "\nresult: " << result;
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_process(QObject* parent)
{
#if (0) //done
    _1::test(parent);
#endif

    _2::test(parent);
}
