#include <QApplication>
#include <QAudioOutput>
#include <QFile>
#include <QMediaPlayer>
#include <QScopedPointer>
#include <QUrl>
#include <QDebug>

#include "TestAudioOutput.h"
#include "TestMediaPlayer.h"

const char* TEST_MP3_FILENAME = "/mnt/share/twice_candy_pop.mp3";
const char* TEST_DIVX_FILENAME = "/mnt/share/divx.divx";
const char* TEST_DOLBY_FILENAME = "/mnt/share/H264_DolbyD 5.1.mp4";
const char* TEST_DTS_FILENAME = "/mnt/share/1920x1080 x.264-10Bit DTS-HDMA.mkv";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->instance();

    auto audioFilename = QString{TEST_MP3_FILENAME};
    auto dolbyAudioFilename = QString{TEST_DOLBY_FILENAME};
    auto divxFilename = QString{TEST_DIVX_FILENAME};
    auto dtsAudioFilename = QString{TEST_DTS_FILENAME};
    auto testfile = dtsAudioFilename;

#if (1)
    //Warning: "No decoder available for type 'video/x-h264, stream-format=(string)avc, alignment=(string)au, level=(string)5.2, profile=(string)high, codec_data=(buffer)01640034ffe1001967640034acd940780227e5c04400000fa40002ee003c60c65801000568ebecf23c, width=(int)1920, height=(int)1080, framerate=(fraction)24000/1001, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive, chroma-format=(string)4:2:0, bit-depth-luma=(uint)8, bit-depth-chroma=(uint)8, parsed=(boolean)true'."
    //Warning: "No decoder available for type 'audio/x-ac3, framed=(boolean)true, rate=(int)48000, channels=(int)6, alignment=(string)frame'."
    //Error: "GStreamer 설치이 플러그인이 누락되었습니다."

    //[1] sudo apt-get install libavcodec-dev
    //[2] sudo apt-get install libgstreamer1.0-dev
    //[3] sudo apt-get install libgstreamer-plugins-base1.0-dev
    //[4] sudo apt-get install libgstreamer-plugins-bad1.0-dev
    //[5] sudo apt-get install gstreamer1.0-plugins-bad
    //[6] sudo apt-get install gstreamer1.0-libav

    QScopedPointer<TestMediaPlayer> mediaPlayer(new TestMediaPlayer);
    mediaPlayer->play(testfile);
#else
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleRate(QAudioFormat::SignedInt);
    format.setSampleSize(16);
    format.setByteOrder(QAudioFormat::LittleEndian);

    QScopedPointer<TestAudioOutput> audioOutput(new TestAudioOutput);
    audioOutput->play(audioFilename, format);
#endif

    return a.exec();
}
