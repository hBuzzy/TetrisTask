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
  connect(figureMoveTimer_, &QTimer::timeout, this, &GameField::StartingGame);

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

Figure GameField::GetnextFigure() { return nextFigure_; }

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

QString GameField::GetScore() {
    return QString::number(score_);
}

void GameField::keyPressEvent(QKeyEvent *event) {
  const uint left = 1;
  const uint right = 2;
  if (event->key() == Qt::Key_Space) {
    if (isGamePaused_ == true) {
      figureMoveTimer_->stop();
      isGamePaused_ = false;
    } else {
        figureMoveTimer_->start(500);
        isGamePaused_ = true;
    }
  } else if (event->key() == Qt::Key_Down) {
      figureMoveTimer_->setInterval(50);
  } else if (event->key() == Qt::Key_Left) {
      currentFigure_.MoveFigure(cellsColorsDump_, left);
  } else if (event->key() == Qt::Key_Right) {
      currentFigure_.MoveFigure(cellsColorsDump_, right);
  }
}


void GameField::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Down) {
    figureMoveTimer_->setInterval(500);
  }
}

void GameField::StartingGame(){
  const uint scoreForLine = 500;
  cellsColors_ = cellsColorsDump_;
  bool isStoped = currentFigure_.StopMoveDown(cellsColors_);
  QVector<QVector<QPair<int, int>>> currCoordinate = currentFigure_.GetCoordinates();
  QVector<QColor> defaultFirstRow;
  defaultFirstRow.resize(columnsNumber_);
  defaultFirstRow.fill(kCellStartColor);

  QColor currColor = currentFigure_.GetColor();
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
     if (countColumn == columnsCount_) {
       score_ += scoreForLine;
       cellsColors_.remove(cellsColors_.indexOf(row));
       cellsColors_.push_front(defaultFirstRow);
     }

     countColumn = 0;
   }

   isEnd_ = isEnd;
   if (isEnd_) {
     figureMoveTimer_->stop();
   }
   currentFigure_ = nextFigure_;
   Figure newFigure;
   nextFigure_ = newFigure;
   cellsColorsDump_ = cellsColors_;
   emit lsdNumberChanged(score_);
   emit clearPrevWindow();
   emit nextFigureChanged(nextFigure_);
  }
}

void GameField::SetCellsColorForStart() {
  for (auto& cell : cellsColors_) {
    cell.fill(kCellStartColor);
  }
  cellsColorsDump_ = cellsColors_;
}

void GameField::StartNewGame() {
  setFocus();
  SetCellsColorForStart();

  qDebug() << "Game Started";
  Figure newFigure;
  nextFigure_ = newFigure;
  score_ = 0;

  figureMoveTimer_->start(500);

  emit nextFigureChanged(nextFigure_);
}

