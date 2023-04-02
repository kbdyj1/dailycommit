#ifndef TESTEXEC_H
#define TESTEXEC_H

#include <QObject>
#include <QProcess>

class TestExec : public QObject
{
    Q_OBJECT

public:
    TestExec(QObject* parent = nullptr);

    virtual ~TestExec();

public slots:
    void start();

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus status);

private:
    QProcess* process = nullptr;
};

#endif // TESTEXEC_H
