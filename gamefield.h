#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>
#include "figure.h"

class GameField : public QWidget {
  Q_OBJECT

  Q_PROPERTY(uint rowsNumber READ GetRowsNumber WRITE SetRowsNumber NOTIFY
                   RowsNumberChanged FINAL)
  Q_PROPERTY(uint columnsNumber READ GetColumnsNumber WRITE SetColumnNumber
                   NOTIFY ColumnsNumberChanged FINAL)

 public:
 explicit GameField(QWidget *parent = nullptr);

 signals:
  void lsdNumberChanged(uint score);
  void nextFigureChanged(Figure nextFigure);
  void clearPrevWindow();
  void RowsNumberChanged();
  void ColumnsNumberChanged();
  void InitialisationStarted();

 public slots:
  void ResetCellsColor();
  void StartNewGame();

 private slots:
  void SetCells();

 protected:
  void paintEvent(QPaintEvent *event);
  QSize GetSize() const;
  void GamePlay();
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  static constexpr uint kCellSize = 20;

 public:
  QString GetScore();
  Figure GetNextFigure();
  QTimer* figureMoveTimer_;
  void SetCellsColorForStart();
  void DrawCells(QPainter *painter);
  uint GetRowsNumber() const;
  void SetRowsNumber(uint newRowsNumber);

  uint GetColumnsNumber() const;
  void SetColumnNumber(uint newColumnsCount);

 private:
  bool gameOnPause_ = false;
  bool isEnd_ = false;
  uint rowsNumber_ = 0;
  uint columnsNumber_ = 0;
  QVector<QVector<QColor>> cellsColors_;
  QVector<QVector<QColor>> cellsColorsTmp_;
  uint rowsCount_ = 0;
  uint columnsCount_ = 0;
  const QColor kCellDefaultColor = QColor(254, 217, 255);
  const QColor kCellStartColor = QColor(254, 217, 255);
  uint score_ = 0;
  Figure currFigure;
  Figure nextFigure;
};

#endif  // GAMEFIELD_H