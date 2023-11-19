#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QLabel>
#include <QPointer>
#include <QTimer>
#include <QWidget>

#include "figure.h"

class GameField : public QWidget {
  Q_OBJECT
 public:
  explicit GameField(QWidget *parent = nullptr);

  Q_PROPERTY(uint rowsNumber READ GetRowsNumber WRITE SetRowsNumber NOTIFY
                 RowsNumberChanged FINAL)
  Q_PROPERTY(uint columnsNumber READ GetColumnsNumber WRITE SetColumnNumber
                 NOTIFY ColumnsNumberChanged FINAL)

 signals:
  void RowsNumberChanged();
  void ColumnsNumberChanged();
  void InitialisationStarted();
  void ScoreChanged(int score);
  void NextFigureGenerated();

 public slots:
  void StartNewGame();
  void ShowNextFigure(QLabel *nextFigurePicture, QLabel *nextFigureLabel_);

 private slots:
  void SetCells();

 public:
  uint GetRowsNumber() const;
  uint GetColumnsNumber() const;
  void SetRowsNumber(uint newRowsNumber);
  void SetColumnNumber(uint newColumnsCount);

  void DrawFigure(QPaintEvent *event);
  void DrawSquare(int x, int y, QColor color, QPainter &painter);
  void SetFigureLabel(QLabel *label);
  void SetNextFigurePicture(QLabel *label);

 protected:
  void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void timerEvent(QTimerEvent *event);

 private:
  void DrawCells(QPainter *painter);
  void DrawFigure(QPainter *painter, Figure &figure, int fieldTop);
  void ResetCellsColor();
  void ClearGameField();
  void RemoveLines();
  void GenerateFigure();
  void MoveFigureOneLineDown();
  void DropCurrentFigureToBottom();
  void FinishFigureDrop();
  void ShowGameOverMessage();
  void SetCurrentFigurePosition(const Figure &newFigure, int newX, int newY);
  void HandleGameOver();
  void UpdateScore(int addScore);
  void ShiftLines();
  void DrawExistingFigures(const QRect &rect, QPainter &painter);
  void DrawCurrentFigure(const QRect &rect, QPainter &painter);

  int CountFullLines();

  bool IsStepPossible(const Figure &newFigure, int newX, int newY);
  bool IsOutOfBounds(int x, int y);
  bool HasCollision(int x, int y);
  bool IsLineFull(int row);

  QSize GetSize() const;
  Shapes &SearchShape(int x, int y);
  QString GetShapeName(Shapes shape);

 private:
  const uint kCellSize_ = 30;
  const QColor kCellDefaultColor_ = QColor(150, 150, 150);

  QVector<QColor> cellsColors_;
  QVector<Shapes> fieldShapes_;
  QLabel *nextFigurePicture_;

  uint rowsNumber_ = 0;
  uint columnsNumber_ = 0;
  uint score_;
  uint intervalTime_ = 500;
  uint currentX_;
  uint currentY_;
  uint numberDroppedPieces_;

  Figure currentFigure_;
  Figure nextFigure_;
  QBasicTimer gameTimer_;

  bool isPlaying_;
  bool isWaiting_;
};

#endif  // GAMEFIELD_H
