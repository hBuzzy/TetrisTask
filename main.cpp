#include "tetrixwindow.h"

#include <QApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TetrixWindow window;
    window.show();
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/tetris_music/background.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(playlist);
    player->setVolume(10);
    player->play();
    return app.exec();
}
