#include <QCoreApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto player = new QMediaPlayer;
    auto audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);

    player->setSource(QUrl::fromLocalFile("/mnt/share/twice_candy_pop.mp3"));
    audioOutput->setVolume(50);

    player->play();

    return a.exec();
}
