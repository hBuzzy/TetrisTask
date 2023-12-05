#ifndef TETRIS_H
#define TETRIS_H

#include <QMainWindow>
#include <QKeyEvent>
#include "gamefield.h"
#include "modaldialog.h"
#include "gameoverdialog.h"
#include "nextfiguregamegrid.h"

#include <QLCDNumber>

QT_BEGIN_NAMESPACE
namespace Ui {
class Tetris;
}
QT_END_NAMESPACE

class Tetris : public QMainWindow {
  Q_OBJECT

 public:
  Tetris(QWidget *parent = nullptr);
  ~Tetris();

  void GamePause();
  void RestoreBasedInterval();
  void UpdateGameField();
  void UpdateWinPoints();
  void StartNewGame();
  void EndGame();
  void OpenModalDialog();
  void OpenEndGameDialog();

private slots:
    void UpdateGameFieldFigure();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Tetris *ui;

    int baseSpeed_, boostSpeed_;
    QTimer *timer_;
    GameField *gamefield_;
    QLCDNumber *scoreCounter_;
    NextFigureGameGrid *nextFigureGrid_;
};
#endif  // TETRIS_H
