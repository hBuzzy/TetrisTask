#include "figure.h"

#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>

Figure::Figure() {
  QRandomGenerator gen1 =  QRandomGenerator(QDateTime::currentMSecsSinceEpoch());
  uint figureType = gen1.bounded(1, 5);
  uint colorType = gen1.bounded(1, 5);
  uint rowCount = 3;
  uint columnCount = 3;
  uint defoultValue = -100;
  for (int i = 0; i < rowCount; i++) {
      QVector<QPair<int, int>> row;
      for (int j = 0; j < columnCount; j++) {
          QPair<int, int> pairValue(0, 0);
          row.append(pairValue);
      }
      coordinate_.append(row);
  }

  switch (figureType) {
    case 1:
      coordinate_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[0][1] = QPair<int, int>(-3, 4);
      coordinate_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[1][1] = QPair<int, int>(-2, 4);
      coordinate_[1][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[2][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[2][1] = QPair<int, int>(-1, 4);
      coordinate_[2][2] = QPair<int, int>(defoultValue, defoultValue);

      break;
    case 2:
      coordinate_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[0][1] = QPair<int, int>(-3, 4);
      coordinate_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[1][1] = QPair<int, int>(-2, 4);
      coordinate_[1][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[2][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[2][1] = QPair<int, int>(-1, 4);
      coordinate_[2][2] = QPair<int, int>(-1, 5);

      break;
    case 3:
      coordinate_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[0][1] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[1][1] = QPair<int, int>(-2, 4);
      coordinate_[1][2] = QPair<int, int>(-2, 5);

      coordinate_[2][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[2][1] = QPair<int, int>(-1, 4);
      coordinate_[2][2] = QPair<int, int>(-1, 5);

      break;
    case 4:
      coordinate_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[0][1] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinate_[1][1] = QPair<int, int>(-2, 4);
      coordinate_[1][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinate_[2][0] = QPair<int, int>(-1, 3);
      coordinate_[2][1] = QPair<int, int>(-1, 4);
      coordinate_[2][2] = QPair<int, int>(-1, 5);

      break;
  }


  switch (colorType) {
    case 1:
      kCellColor = QColor(255, 0, 0);
      break;
    case 2:
      kCellColor = QColor(253, 113, 163);
      break;
    case 3:
      kCellColor = QColor(0, 0, 255);
      break;
    case 4:
      kCellColor = QColor(255, 0, 255);
      break;
  }
}

QColor Figure::GetColor() { return kCellColor; }

QVector<QVector<QPair<int, int>>> Figure::GetCoordinates() { return coordinate_; }

void Figure::MoveLeft(QVector<QVector<QColor>> coordinateGlobal) {
  if (CheckColisium(coordinateGlobal, 1)) {
    for (auto& row : coordinate_) {
      for (auto& cell : row) {
          if (cell == QPair<int, int>(0, 0)) continue;
          cell.second -= 1;
      }
    }
  }
}

void Figure::MoveRight(QVector<QVector<QColor>> coordinateGlobal) {
  if (CheckColisium(coordinateGlobal, 2)) {
    for (auto& row : coordinate_) {
      for (auto& cell : row) {
          if (cell == QPair<int, int>(0, 0)) continue;
          cell.second += 1;
      }
    }
  }
}

bool Figure::MoveDown(QVector<QVector<QColor>> coordinateGlobal) {
  if (CheckColisium(coordinateGlobal, 3)) {
    for (auto& row : coordinate_) {
      for (auto& cell : row) {
          if (cell == QPair<int, int>(0, 0)) continue;
          cell.first += 1;
      }
    }
  } else return true;
  return false;
}

void Figure::RotateFigure(QVector<QVector<QColor>> coordinateGlobal) {
  QVector<QVector<QPair<uint, uint>>> rotatedCoordinate(coordinate_[0].size(), QVector<QPair<uint, uint>>(coordinate_.size()));

  for (uint i = 0; i < coordinate_.size(); i++) {
    for (uint j = 0; j < coordinate_[0].size(); j++) {
      rotatedCoordinate[j][coordinate_.size() - i - 1] = coordinate_[i][j];
    }
  }

  for (int i = 0; i < coordinate_.size(); i++) {
    for (int j = 0; j < coordinate_[0].size(); j++) {
      coordinate_[i][j] = rotatedCoordinate[i][j];
    }
  }
}

bool Figure::CheckColisium(QVector<QVector<QColor>> coordinateGlobal, int side) {
  QPair<int, int> defaultValue(-100, -100);
  const QColor kCellStartColor = QColor(220, 220, 220);
  switch (side) {
    case(1):
      for (auto& row : coordinate_) {
        for (auto& cell : row) {
          if (cell == defaultValue) continue;
          if (cell.first < 0) continue;
          if (cell.second <= 0) { return false; }
          if (coordinateGlobal[cell.first][cell.second - 1] != kCellStartColor)
            return false;
        }
      }
      return true;
      break;

    case(2):
      for (auto& row : coordinate_) {
        for (auto& cell : row) {
          if (cell == defaultValue) continue;
          if (cell.first < 0) continue;
          if (cell.second >= 9) { return false; }
          if (coordinateGlobal[cell.first][cell.second + 1] != kCellStartColor) { return false; }
        }
      }
        return true;
        break;

    case(3):
      for (auto& row : coordinate_) {
        for (auto& cell : row) {
          if (cell == defaultValue) continue;
          if (cell.first < 0) continue;
          if (cell.first == 19) { return false; }
          if ((coordinateGlobal[cell.first + 1][cell.second] != kCellStartColor)) { return false; }
        }
      }
        return true;
        break;
    }
}


