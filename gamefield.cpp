#include "gamefield.h"

#include <math.h>

#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <QTimerEvent>
#include <QWidget>

GameField::GameField(QWidget *parent) : QWidget{parent} {
  connect(this, &GameField::InitialisationStarted, this, &GameField::SetCells,
          Qt::QueuedConnection);
  isPlaying_ = false;
  ResetCellsColor();
  nextFigure_.RandomizeFigure();

  emit NextFigureGenerated();
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
  QWidget::paintEvent(event);

  QRect rect = contentsRect();
  QPainter painter(this);
  painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
  DrawCells(&painter);

  int fieldTop = rect.bottom() - rowsNumber_ * kCellSize_;

  for (uint i = 0; i < rowsNumber_; ++i) {
    for (uint j = 0; j < columnsNumber_; ++j) {
      Shapes shape = SearchShape(j, rowsNumber_ - i - 1);
      if (shape != NoShape)
        DrawSquare(rect.left() + j * kCellSize_, fieldTop + i * kCellSize_,
                   nextFigure_.KColors_[static_cast<int>(shape)], painter);
    }
  }

  int numberOfCells = currentFigure_.GetNumberOfCells();
  int numberNoShape = 0;

  if (currentFigure_.GetShape() != numberNoShape) {
    for (int i = 0; i < numberOfCells; ++i) {
      int x = currentX_ + currentFigure_.GetX(i);
      int y = currentY_ - currentFigure_.GetY(i);
      DrawSquare(rect.left() + x * kCellSize_,
                 fieldTop + (rowsNumber_ - y - 1) * kCellSize_,
                 currentFigure_.GetColor(), painter);
    }
  }
}

void GameField::keyPressEvent(QKeyEvent *event) {
  if (!isPlaying_ || currentFigure_.GetShape() == NoShape) {
    QWidget::keyPressEvent(event);
    return;
  }

  bool clockwise = true;

  switch (event->key()) {
    case Qt::Key_Left:
      IsStepPossible(currentFigure_, currentX_ - 1, currentY_);
      break;
    case Qt::Key_Right:
      IsStepPossible(currentFigure_, currentX_ + 1, currentY_);
      break;
    case Qt::Key_Down:
      IsStepPossible(currentFigure_.Rotate(clockwise), currentX_, currentY_);
      break;
    case Qt::Key_Up:
      IsStepPossible(currentFigure_.Rotate(!clockwise), currentX_, currentY_);
      break;
    case Qt::Key_Space:
      DropCurrentFigureToBottom();
      break;
    case Qt::Key_V:
      MoveFigureOneLineDown();
      break;
    default:
      QWidget::keyPressEvent(event);
  }
  update();
}

void GameField::timerEvent(QTimerEvent *event) {
  if (event->timerId() == gameTimer_.timerId()) {
    if (isWaiting_) {
      isWaiting_ = false;
      GenerateFigure();
      gameTimer_.start(intervalTime_, this);
    } else {
      MoveFigureOneLineDown();
    }
  } else {
    QWidget::timerEvent(event);
  }
}

void GameField::DrawCells(QPainter *painter) {
  int positionX = 0;
  int positionY = 0;

  for (uint i = 0; i < rowsNumber_; i++) {
    for (uint j = 0; j < columnsNumber_; j++) {
      painter->setBrush(kCellDefaultColor_);
      painter->drawRect(positionX, positionY, positionX + kCellSize_,
                        positionY + kCellSize_);
      positionX += kCellSize_;
    }

    positionX = 0;
    positionY += kCellSize_;
  }
}

void GameField::SetCells() {
  fieldShapes_.resize(rowsNumber_ * columnsNumber_);
  cellsColors_.resize(rowsNumber_ * columnsNumber_);
  ResetCellsColor();
  setFixedSize(GetSize());
}

void GameField::StartNewGame() {
  setFocus();

  isPlaying_ = true;
  isWaiting_ = false;
  score_ = 0;
  numberDroppedPieces_ = 0;
  ClearGameField();
  ResetCellsColor();

  emit ScoreChanged(score_);
  GenerateFigure();
  gameTimer_.start(intervalTime_, this);
  qDebug() << "new game started";
}

void GameField::ResetCellsColor() { cellsColors_.fill(kCellDefaultColor_); }

void GameField::ClearGameField() { fieldShapes_.fill(NoShape); }

QSize GameField::GetSize() const {
  return QSize(columnsNumber_, rowsNumber_) * kCellSize_;
}

Shapes &GameField::SearchShape(int x, int y) {
  return fieldShapes_[(y * columnsNumber_) + x];
}

QString GameField::GetShapeName(Shapes shape) {
  switch (shape) {
    case I1:
      return "I1";
    case I2:
      return "I2";
    case I3:
      return "I3";
    case I4:
      return "I4";
    case LShape:
      return "LShape";
    case Square:
      return "Square";
    default:
      return "NoShape";
  }
}

void GameField::DrawSquare(int x, int y, QColor color, QPainter &painter) {
  painter.fillRect(x + 1, y + 1, kCellSize_ - 2, kCellSize_ - 2, color);

  painter.setPen(color.lighter());
  painter.drawLine(x, y + kCellSize_ - 1, x, y);
  painter.drawLine(x, y, x + kCellSize_ - 1, y);

  painter.setPen(color.darker());
  painter.drawLine(x + 1, y + kCellSize_ - 1, x + kCellSize_ - 1,
                   y + kCellSize_ - 1);
  painter.drawLine(x + kCellSize_ - 1, y + kCellSize_ - 1, x + kCellSize_ - 1,
                   y + 1);
}

void GameField::DropCurrentFigureToBottom() {
  int dropHeight = 0;
  int newY = currentY_;
  while (newY > 0) {
    if (!IsStepPossible(currentFigure_, currentX_, newY - 1)) break;
    --newY;
    ++dropHeight;
  }
  FinishFigureDrop();
}

void GameField::MoveFigureOneLineDown() {
  if (!IsStepPossible(currentFigure_, currentX_, currentY_ - 1))
    FinishFigureDrop();
}

void GameField::FinishFigureDrop() {
  for (int i = 0; i < 4; ++i) {
    int x = currentX_ + currentFigure_.GetX(i);
    int y = currentY_ - currentFigure_.GetY(i);
    SearchShape(x, y) = currentFigure_.GetShape();
  }
  ++numberDroppedPieces_;
  RemoveLines();
  if (!isWaiting_) GenerateFigure();
}

void GameField::ShowNextFigure(QLabel *nextFigurePicture,
                               QLabel *nextFigureLabel) {
  if (!nextFigurePicture) return;

  int dx = nextFigure_.GetMaxX() - nextFigure_.GetMinX() + 1;
  int dy = nextFigure_.GetMaxY() - nextFigure_.GetMinY() + 1;

  QPixmap pixmap(dx * kCellSize_, dy * kCellSize_);
  QPainter painter(&pixmap);
  painter.fillRect(pixmap.rect(), nextFigurePicture->palette().background());

  for (int i = 0; i < 4; ++i) {
    int x = nextFigure_.GetX(i) - nextFigure_.GetMinX();
    int y = nextFigure_.GetY(i) - nextFigure_.GetMinY();
    DrawSquare(x * kCellSize_, y * kCellSize_, nextFigure_.GetColor(), painter);
  }
  nextFigurePicture->setPixmap(pixmap);
  nextFigureLabel->setText(GetShapeName(nextFigure_.GetShape()));
}

void GameField::RemoveLines() {
  int numberOfFullLines = CountFullLines();
  int addScore = 10;

  if (numberOfFullLines > 0) {
    UpdateScore(addScore);
    ShiftLines();
    gameTimer_.start(intervalTime_, this);
    isWaiting_ = true;
    currentFigure_.SetShape(NoShape);
    update();
  }
}

void GameField::GenerateFigure() {
  currentFigure_ = nextFigure_;
  nextFigure_.RandomizeFigure();
  currentX_ = columnsNumber_ / 2;
  currentY_ = rowsNumber_ + currentFigure_.GetMinY() - 1;

  if (!IsStepPossible(currentFigure_, currentX_, currentY_)) {
    HandleGameOver();
    return;
  }

  emit NextFigureGenerated();
}

bool GameField::IsOutOfBounds(int x, int y) {
  return x < 0 || x >= columnsNumber_ || y < 0;
}

bool GameField::HasCollision(int x, int y) {
  return SearchShape(x, y) != NoShape;
}

void GameField::SetCurrentFigurePosition(const Figure &newFigure, int newX,
                                         int newY) {
  currentFigure_ = newFigure;
  currentX_ = newX;
  currentY_ = newY;
  update();
}

void GameField::HandleGameOver() {
  currentFigure_.SetShape(NoShape);
  gameTimer_.stop();
  isPlaying_ = false;

  ShowGameOverMessage();
}

int GameField::CountFullLines() {
  int count = 0;

  for (int i = rowsNumber_ - 1; i >= 0; --i) {
    if (IsLineFull(i)) {
      ++count;
    }
  }

  return count;
}

void GameField::UpdateScore(int addScore) {
  score_ += addScore;
  emit ScoreChanged(score_);
}

void GameField::ShiftLines() {
  for (int i = 0; i < rowsNumber_ - 1; ++i) {
    for (uint j = 0; j < columnsNumber_; ++j) {
      SearchShape(j, i) = SearchShape(j, i + 1);
    }
  }

  for (uint j = 0; j < columnsNumber_; ++j) {
    SearchShape(j, rowsNumber_ - 1) = NoShape;
  }
}

bool GameField::IsLineFull(int row) {
  for (uint j = 0; j < columnsNumber_; ++j) {
    if (SearchShape(j, row) == NoShape) {
      return false;
    }
  }

  return true;
}

void GameField::ShowGameOverMessage() {
  QMessageBox::information(this, "Game Over", "Oops 😢. Попробуйте еще раз 🙀");
}

bool GameField::IsStepPossible(const Figure &newFigure, int newX, int newY) {
  for (int i = 0; i < 4; ++i) {
    int x = newX + newFigure.GetX(i);
    int y = newY - newFigure.GetY(i);

    if (IsOutOfBounds(x, y) || HasCollision(x, y)) {
      return false;
    }
  }

  SetCurrentFigurePosition(newFigure, newX, newY);
  return true;
}
