#include "previewwindow.h"

#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

PreviewWindow::PreviewWindow(QWidget *parent) : QWidget{parent} {
  connect(this, &PreviewWindow::InitialisationStarted, this, &PreviewWindow::SetCells,
          Qt::QueuedConnection);

  emit InitialisationStarted();
}

uint PreviewWindow::GetRowsNumber() const { return rowsNumber_; }

void PreviewWindow::SetRowsNumber(uint rowsNumber) {
  if (rowsNumber_ == rowsNumber) return;

  rowsNumber_ = rowsNumber;
  emit RowsNumberChanged();
}

uint PreviewWindow::GetColumnsNumber() const { return columnsNumber_; }

void PreviewWindow::SetColumnNumber(uint columnsCount) {
  if (columnsNumber_ == columnsCount) return;

  columnsNumber_ = columnsCount;
  emit ColumnsNumberChanged();
}

void PreviewWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
  DrawCells(&painter);
}

void PreviewWindow::SetCells() {
  cellsColors_.resize(rowsNumber_);

  for (uint i = 0; i < rowsNumber_; i++) {
    cellsColors_[i].resize(columnsNumber_);
  }

  ResetCellsColor();
  setFixedSize(GetSize());
}

void PreviewWindow::ResetCellsColor() {
  for (auto& cell : cellsColors_) {
    cell.fill(kCellDefaultColor);
  }
}

QSize PreviewWindow::GetSize() const {
  return QSize(columnsNumber_, rowsNumber_) * kCellSize;
}

void PreviewWindow::DrawCells(QPainter *painter) {
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

void PreviewWindow::StartNewGame(Figure nextFigure) {
  Figure newFig;
  score_ = 0;
  QColor currColor = nextFigure.GetColor();
  for (auto& row : nextFigure.GetCoordinate()) {
    for (auto& cell : row) {
      if (cell.first == -100) continue;
      cellsColors_[abs(cell.first)  - 1][cell.second - 3] = currColor;
    }
  }
  repaint();
}

//void GameField::lsdNumberChanged() { return score_; };

