#ifndef TETRISVIEW_H
#define TETRISVIEW_H

#include <tetriscontroller.h>
#include <tetrismodel.h>

#include <QApplication>
#include <QKeyEvent>
#include <QLCDNumber>
#include <QLabel>
#include <QPainter>
#include <QWidget>

class TetrisView : public QWidget {
  Q_OBJECT

  Q_PROPERTY(uint rowsNumber READ GetRowsNumber WRITE SetRowsNumber NOTIFY
                 RowsNumberChanged FINAL)

  Q_PROPERTY(uint columnsNumber READ GetColumnsNumber WRITE SetColumnsNumber
                 NOTIFY ColumnsNumberChanged FINAL)
 signals:
  void RowsNumberChanged();
  void ColumnsNumberChanged();
  void InitialisationStarted();

 private slots:
  void SetCells();

 public:
  explicit TetrisView(QWidget *parent = 0);
  ~TetrisView();
  void SetNextFigureName(int);
  void Refresh();
  uint GetRowsNumber() const;
  void SetRowsNumber(uint newRowsNumber);
  uint GetColumnsNumber() const;
  void SetColumnsNumber(uint newColumnsCount);

 protected:
  void paintEvent(QPaintEvent *);
  void keyPressEvent(QKeyEvent *pressedKey);
  void keyReleaseEvent(QKeyEvent *e);
  void DrawBlock(int xPoints, int yPoints, int type, QPainter *painter);

 private:
  TetrisModel *model_;
  TetrisController *controller_;
  QLCDNumber *scoreNumber_;
  QLabel *nextFigureName_;
  bool IsGameOver_;
  int width_;
  int height_;
  int width_extension_;
  uint rowsNumber_ = 15;
  uint columnsNumber_ = 25;
};

#endif  // TETRISVIEW_H
