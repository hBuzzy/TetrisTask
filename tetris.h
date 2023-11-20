#ifndef TETRIS_H
#define TETRIS_H

#include <QLabel>
#include <QMainWindow>

#include "ui_tetris.h"

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

 private:
  QLabel *nextFigureLabel_;
  GameField *field_;

  Ui::Tetris *ui;
  void ShowHelp();
};
#endif  // TETRIS_H
