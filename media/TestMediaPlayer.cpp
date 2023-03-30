#include "TestMediaPlayer.h"
#include <QVideoWidget>

TestMediaPlayer::TestMediaPlayer(QObject* parent)
    : QObject(parent)
    , player(new QMediaPlayer(this))
{}

void TestMediaPlayer::play(const QString &filename)
{
    auto* widget = new QVideoWidget();
    widget->setGeometry(0, 0, 720, 576);
    widget->show();

    player->setMedia(QUrl::fromLocalFile(filename));
    player->setVideoOutput(widget);
    player->play();
}
