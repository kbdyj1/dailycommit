#ifndef TESTAUDIOOUTPUT_H
#define TESTAUDIOOUTPUT_H

#include <QObject>
#include <QFile>
#include <QAudio>

class QAudioFormat;
class QAudioOutput;
class QAudioDecoder;

class TestAudioOutput : public QObject
{
    Q_OBJECT

public:
    TestAudioOutput(QObject* parent = nullptr);

    void play(const QString& filename, const QAudioFormat& format);
    bool checkSupport(const QAudioFormat& format);

private Q_SLOTS:
    void handleStateChanged(QAudio::State);

private:
    QFile file;
    QAudioOutput* audioOutput = nullptr;
    QAudioDecoder* decoder = nullptr;
};

#endif // TESTAUDIOOUTPUT_H
