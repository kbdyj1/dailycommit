#ifndef TESTMEDIAPLAYER_H
#define TESTMEDIAPLAYER_H

#include <QObject>
#include <QScopedPointer>
#include <QMediaPlayer>

class QMediaPlayer;

class TestMediaPlayer : public QObject
{
    Q_OBJECT

public:
    TestMediaPlayer(QObject* parent = nullptr);

    void play(const QString& filename);

private:
    QScopedPointer<QMediaPlayer> player;
};

#endif // TESTMEDIAPLAYER_H
