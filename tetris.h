#ifndef TETRIS_H
#define TETRIS_H

#include <QMainWindow>
#include <QPushButton>
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

private slots:
  void showRules();

 private:
  Ui::Tetris *ui;
  QMenu *mewGameMenu_;
  QMenu *exitMenu_;
  QMenu *rulesMenu_;
};
#endif  // TETRIS_H
