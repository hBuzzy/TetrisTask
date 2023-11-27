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

uint GameField::GetColumnsNumber() const { return columnsNumber_; }

Figure GameField::GetnextFigure() { return nextFigure_; }

QSize GameField::GetSize() const { return QSize(columnsNumber_, rowsNumber_) * kCellSize; }

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

void GameField::SetColumnNumber(uint columnsCount) {
  if (columnsNumber_ == columnsCount) return;

  columnsNumber_ = columnsCount;
  emit ColumnsNumberChanged();
}

void GameField::SetRowsNumber(uint rowsNumber) {
  if (rowsNumber_ == rowsNumber) return;

  rowsNumber_ = rowsNumber;
  emit RowsNumberChanged();
}

void GameField::SetCells() {
  cellsColors_.resize(rowsNumber_);

  for (uint i = 0; i < rowsNumber_; i++) {
    cellsColors_[i].resize(columnsNumber_);
  }

  firstRow_.resize(rowsNumber_);
  firstRow_.fill(kCellStartColor);

  ResetCellsColor();
  setFixedSize(GetSize());
}

void GameField::SetCellsColorForStart() {
  for (auto& cell : cellsColors_) {
    cell.fill(kCellStartColor);
  }
  cellsColorsDump_ = cellsColors_;
}

void GameField::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
  DrawCells(&painter);
}

void GameField::ResetCellsColor() {
  for (auto& cell : cellsColors_) {
     cell.fill(kCellDefaultColor);
  }
}

void GameField::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    if (IsGameOnPause_ == true) {
      figureMoveTimer_->stop();
      IsGameOnPause_ = false;
    } else {
        figureMoveTimer_->start(500);
        IsGameOnPause_ = true;
    }
  } else if (event->key() == Qt::Key_Down) {
      figureMoveTimer_->setInterval(50);
  } else if (event->key() == Qt::Key_Left) {
      currFigure_.MoveLeft(cellsColorsDump_);
      repaint();
  } else if (event->key() == Qt::Key_Right) {
      currFigure_.MoveRight(cellsColorsDump_);
      repaint();
  } else if (event->key() == Qt::Key_Up) {
      currFigure_.RotateFigure(cellsColorsDump_);
      repaint();
  }
}


void GameField::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Down) {
    figureMoveTimer_->setInterval(500);
  }
}

void GameField::UpdateGameField(){
  cellsColors_ = cellsColorsDump_;
  bool isStoped = currFigure_.MoveDown(cellsColors_);
  QVector<QVector<QPair<int, int>>> currCoordinate = currFigure_.GetCoordinates();
  qDebug() << currCoordinate;
  QColor currColor = currFigure_.GetColor();
  for (auto& row : currCoordinate) {
    for (auto& cell : row) {
      if (cell.first < 0) continue;
      cellsColors_[cell.first][cell.second] = currColor;
    }
  }
  repaint();
  if (isStoped) {
    bool IsGameover = true;
    for (auto& row : currCoordinate) {
      for (auto& cell : row) {
        if (cell.first > 0) IsGameover = false;
      }
    }

   uint countColumns = 0;
   for (auto& row : cellsColors_) {
     for (auto& cell: row) {
       if(cell != kCellStartColor) { countColumns++; }
     }
     if (countColumns == columnsCount_) {
       totalScore_ += 500;
       cellsColors_.remove(cellsColors_.indexOf(row));
       cellsColors_.push_front(firstRow_);
     }

     countColumns = 0;
   }

   IsGameover_ = IsGameover;
   if (IsGameover_) {
     figureMoveTimer_->stop();
   }
   currFigure_ = nextFigure_;
   Figure newFig;
   nextFigure_ = newFig;
   cellsColorsDump_ = cellsColors_;
   emit lsdNumberChanged(totalScore_);
   emit clearPrevWindow();
   emit nextFigureChanged(nextFigure_);
  }
}

void GameField::StartNewGame() {
  setFocus();
  SetCellsColorForStart();

  Figure newFig;
  nextFigure_ = newFig;
  totalScore_ = 0;

  figureMoveTimer_->start(500);

  emit nextFigureChanged(nextFigure_);
}

