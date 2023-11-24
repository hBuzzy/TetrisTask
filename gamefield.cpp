#include "gamefield.h"

#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <cmath>

GameField::GameField(QWidget *parent) : QWidget{parent} {
  setFocusPolicy(Qt::StrongFocus);
  figureMoveTimer_ = new QTimer(this);

  connect(this, &GameField::InitialisationStarted, this, &GameField::SetCells,
          Qt::QueuedConnection);
  connect(figureMoveTimer_, &QTimer::timeout, this, &GameField::UpdateGameField);

  emit InitialisationStarted();
}

uint GameField::GetRowsNumber() const { return rowsNumber_; }

void GameField::SetRowsNumber(uint rowsNumber) {
  if (rowsNumber_ == rowsNumber) return;

  rowsNumber_ = rowsNumber;
  emit RowsNumberChanged();
}

uint GameField::GetColumnsNumber() const { return columnsNumber_; }

void GameField::SetColumnNumber(uint columnsCount) {
  if (columnsNumber_ == columnsCount) return;

  columnsNumber_ = columnsCount;
  emit ColumnsNumberChanged();
}

void GameField::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
  DrawCells(&painter);
}

void GameField::SetCells() {
  cellsColors_.resize(rowsNumber_);

  for (uint i = 0; i < rowsNumber_; i++) {
    cellsColors_[i].resize(columnsNumber_);
  }

  ResetCellsColor();
  setFixedSize(GetSize());
}

void GameField::ResetCellsColor() {
  for (auto& cell : cellsColors_) {
     cell.fill(kCellDefaultColor);
  }
}

Figure GameField::GetNextFigure() { return nextFigure; }

QSize GameField::GetSize() const {
  return QSize(columnsNumber_, rowsNumber_) * kCellSize;
}

void GameField::DrawCells(QPainter *painter) {
  int positionX = 0;
  int positionY = 0;
  for (uint i = 0; i < rowsNumber_; i++) {
    for (uint j = 0; j < columnsNumber_; j++) {
      painter->setBrush(cellsColors_[i][j]);
      painter->drawRect(positionX, positionY, kCellSize, kCellSize);

      positionX += kCellSize;
    }
    positionX = 0;
    positionY += kCellSize;
  }
}

void GameField::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    if (gameOnPause_ == true) {
      figureMoveTimer_->stop();
      gameOnPause_ = false;
    } else {
        figureMoveTimer_->start(500);
        gameOnPause_ = true;
    }
  } else if (event->key() == Qt::Key_Down) {
      figureMoveTimer_->setInterval(50);
  } else if (event->key() == Qt::Key_Left) {
      currFigure.MoveLeft(cellsColorsTmp_);
      repaint();
  } else if (event->key() == Qt::Key_Right) {
      currFigure.MoveRight(cellsColorsTmp_);
      repaint();
  } else if (event->key() == Qt::Key_Up) {
      currFigure.RotateFigure(cellsColorsTmp_);
      repaint();
  }
}


void GameField::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Down) {
    figureMoveTimer_->setInterval(500);
  }
}

void GameField::UpdateGameField(){
  cellsColors_ = cellsColorsTmp_;
  bool isStoped = currFigure.MoveDown(cellsColors_);
  QVector<QVector<QPair<int, int>>> currCoordinate = currFigure.GetCoordinates();
  qDebug() << currCoordinate;
  QColor currColor = currFigure.GetColor();
  for (auto& row : currCoordinate) {
    for (auto& cell : row) {
      if (cell.first < 0) continue;
      cellsColors_[cell.first][cell.second] = currColor;
    }
  }
  repaint();
  if (isStoped) {
    bool isEnd = true;
    for (auto& row : currCoordinate) {
      for (auto& cell : row) {
        if (cell.first > 0) isEnd = false;
      }
    }

   uint countColumn = 0;
   for (auto& row : cellsColors_) {
     for (auto& cell: row) {
       if(cell != kCellStartColor) { countColumn++; }
     }
     if (countColumn == 10) {
       score_ += 500;
       cellsColors_.remove(cellsColors_.indexOf(row));
       cellsColors_.push_front(QVector<QColor>{kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor, kCellStartColor});
     }

     countColumn = 0;
   }

   isEnd_ = isEnd;
   if (isEnd_) {
     figureMoveTimer_->stop();
   }
   currFigure = nextFigure;
   Figure newFig;
   nextFigure = newFig;
   cellsColorsTmp_ = cellsColors_;
   emit lsdNumberChanged(score_);
   emit clearPrevWindow();
   emit nextFigureChanged(nextFigure);
  }
}

void GameField::SetCellsColorForStart() {
  for (auto& cell : cellsColors_) {
    cell.fill(kCellStartColor);
  }
  cellsColorsTmp_ = cellsColors_;
}

void GameField::StartNewGame() {
  setFocus();
  SetCellsColorForStart();

  qDebug() << "Game Started";
  Figure newFig;
  nextFigure = newFig;
  score_ = 0;

  figureMoveTimer_->start(500);

  emit nextFigureChanged(nextFigure);
}

