#ifndef TETRIS_H
#define TETRIS_H

#include <QMainWindow>
#include <QKeyEvent>
#include "gamefield.h"
#include "modaldialog.h"
#include "gameoverdialog.h"
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

  void gamePause();
  void restoreBasedInterval();
  void updateGameField();
  void updateWinPoints();
  void startNewGame();
  void endGame();

 private:
  Ui::Tetris *ui;

    QTimer *timer;
    GameField *gamefield;
    ModalDialog *modaldialog;
    QLCDNumber *lcdNumber;
    void openModalDialog();
    void openEndGameDialog();

    void keyPressEvent(QKeyEvent *event);

};
#endif  // TETRIS_H
