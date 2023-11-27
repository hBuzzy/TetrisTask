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
  void UpdateGameField();
  void SetCells();

 protected:
  void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

 public:
  QSize GetSize() const;
  Figure GetNextFigure();
  QTimer* figureMoveTimer_;
  void SetCellsColorForStart();
  void DrawCells(QPainter *painter);
  uint GetRowsNumber() const;
  void SetRowsNumber(uint newRowsNumber);
  uint GetColumnsNumber() const;
  void SetColumnNumber(uint newColumnsCount);

 private:
  static constexpr uint kCellSize = 20;
  bool IsGameOnPause_ = false;
  bool IsGameover_ = false;
  uint rowsNumber_ = 0;
  uint columnsNumber_ = 0;
  QVector<QVector<QColor>> cellsColors_;
  QVector<QVector<QColor>> cellsColorsDump_;
  QVector<QColor> firstRow_;
  uint rowsCount_ = 0;
  uint columnsCount_ = 0;
  const QColor kCellDefaultColor = QColor(150, 150, 150);
  const QColor kCellStartColor = QColor(220, 220, 220);
  uint totalScore_ = 0;
  Figure currFigure_;
  Figure nextFigure_;
};

#endif  // GAMEFIELD_H
