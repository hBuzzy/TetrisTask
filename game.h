#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <time.h>
#include <QMessageBox>
#include <QPainter>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QMediaPlayer>

const int BLOCK_SIZE = 25;
const int MARGIN     = 5 ;
const int AREA_ROW   = 20;
const int AREA_COL   = 12;
const int FONT_SIZE  = 10;

//direction
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};

struct Border
{
    int ubound;
    int dbound;
    int lbound;
    int rbound;
};

struct block_point
{
    int pos_x;
    int pos_y;
};

enum GameState
{
    FreeRun = 1,
    Wait = 2,
    Game_Over = 3
};

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    void InitGame();
    void StartGame();
    void GameOver();

    void ResetBlock();
    void BlockMove(Direction dir);
    void BlockRotate(int block[4][4]);
    void CreateBlock(int block[4][4],int block_id);
    void GetBorder(int block[4][4],Border &border);
    void ConvertStable(int x,int y);
    void CopyCurrentBlockToCurrent();
    void GetCurrentBlockBorder();
    void SetStartPositionForNewBlock();
    void CreateNextBlock();
    void DrawBackground(QPainter &painter);
    void DrawNextBlock(QPainter &painter);
    void DrawScore(QPainter &painter);
    void DrawGameArea(QPainter &painter);
    void DrawBlock(QPainter &painter, const QColor &color, int x, int y);
    void MusicStateChanged(QMediaPlayer::State state);
    void MoveBlockUp();
    void MoveBlockRight();
    void MoveBlockLeft();
    void MoveBlockDown();
    void MoveBlockSpace();
    void handleGameOver();
    void toggleMusic();

    bool IsCollide(int x,int y,Direction dir);

public:
    explicit Game(QWidget *parent = 0);
    ~Game();

    virtual void paintEvent(QPaintEvent *event);
    virtual void timerEvent(QTimerEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
private:
    Ui::Game *ui;

private:
    GameState game_state_;
    int game_area_[AREA_ROW][AREA_COL];
    block_point block_position_;
    int current_block_[4][4];
    Border current_border_;
    int next_block_[4][4];
    int score_;
    int game_timer_;
    int paint_timer_;
    int speed_ms_;
    int refresh_ms_;

    QMediaPlayer* music_player_;

    static const int font_size_title = 10;
    static const int default_font_size = 10;
    static const int small_font_size = 8;
    const QString default_font_family = "Default";

    void startMusicPlayer();
};

#endif // GAME_H
