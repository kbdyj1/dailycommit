#include "TestAudioOutput.h"
#include <QAudioDecoder>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QDebug>

TestAudioOutput::TestAudioOutput(QObject* parent) : QObject(parent)
{}

void TestAudioOutput::play(const QString &filename, const QAudioFormat& format)
{
#if (1)
    if (audioOutput) {
        delete audioOutput;
    }
    audioOutput = new QAudioOutput(format, this);

    connect(audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    audioOutput->start(&file);
#else
    if (decoder) {
        delete decoder;
    }
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(format);
    decoder->setSourceFilename(filename);
    decoder->start();
#endif
}

bool TestAudioOutput::checkSupport(const QAudioFormat &format)
{
    auto info = QAudioDeviceInfo::defaultInputDevice();

    return info.isFormatSupported(format);
}

void TestAudioOutput::handleStateChanged(QAudio::State state)
{
    auto str = QString{};

    switch (state)
    {
    case QAudio::ActiveState:       str = "Active";     break;
    case QAudio::SuspendedState:    str = "Suspend";    break;
    case QAudio::StoppedState:      str = "Stopped";    break;
    case QAudio::IdleState:         str = "Idle";       break;
    case QAudio::InterruptedState:  str = "Interrupted";break;
    }

    qDebug() << "handleStateChanged(" << str << ")";
}
