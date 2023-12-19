#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPoint>

#include "figure.h"

class GameField : public QWidget {
  Q_OBJECT
 public:
  explicit GameField(QWidget *parent = nullptr);

  Q_PROPERTY(uint rowsNumber READ GetRowsNumber WRITE SetRowsNumber NOTIFY
                 rowsNumberChanged FINAL)
  Q_PROPERTY(uint columnsNumber READ GetColumnsNumber WRITE SetColumnNumber
                 NOTIFY columnsNumberChanged FINAL)

 signals:
  void rowsNumberChanged();
  void columnsNumberChanged();
  void initialisationStarted();
  void moveFigure();

 private slots:
  void SetCells();

 public:
  uint GetRowsNumber() const;
  uint GetColumnsNumber() const;
  void SetColumnNumber(uint newColumnsCount);
  void SetRowsNumber(uint newRowsNumber);

  void moveDown();
  void rotateFigure();
  void moveFigureLeft();
  void moveFigureRight();
  void setNewFigure();
  void startNewGame();
  void clearLines();
  void changeScore(int points);

  QVector<QVector<int>> getGameField();

protected slots:
    void refresh();

 private:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool isCollision(const Figure &movedFigure, int xOffset, int yOffset);
    void drawFigure(Figure figure, QPainter *painter, QPoint currentPosition);

    QVector<QVector<int>> gameField_;
    Figure  nextFigure_;
    Figure currentFigure_;
    uint rowsNumber_ = 0;
    uint columnsNumber_ = 0;
    int currentXPosition_ = 3;
    int currentYPosition_ = 0;
    int score_ = 0;
    int blockSize_ = 30;
    QTimer* timer_;
    QPainter painter_;
};

#endif  // GAMEFIELD_H
