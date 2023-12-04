#include "tetrixwindow.h"

#include <QApplication>
#include <QMediaPlayer>
#include <QDir>
#include <QUrl>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TetrixWindow window;
    window.show();
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/Video_Game_Players_-_Tetris_Theme_48152782.mp3"));
    player->setVolume(100);
    player->play();
    return app.exec();
}
