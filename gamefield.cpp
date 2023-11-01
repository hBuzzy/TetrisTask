#include "gamefield.h"

#include <QDebug>

GameField::GameField(QWidget *parent) : QWidget{parent} {
  connect(this, &GameField::InitialisationStarted, this, &GameField::SetCells,
          Qt::QueuedConnection);

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

void GameField::SetCells() {
  qDebug() << rowsNumber_ << " " << columnsNumber_ << "|";
}
