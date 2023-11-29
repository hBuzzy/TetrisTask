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

  const uint figureStick = 1;
  const uint figureL = 2;
  const uint figureSqr = 3;
  const uint figureT = 4;

  for (int i = 0; i < rowCount; i++) {
      QVector<QPair<int, int>> row;
      for (int j = 0; j < columnCount; j++) {
          QPair<int, int> pairValue(0, 0);
          row.append(pairValue);
      }
      coordinates_.append(row);
  }

  switch (figureType) {
    case figureStick:
      coordinates_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[0][1] = QPair<int, int>(-3, 4);
      coordinates_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[1][1] = QPair<int, int>(-2, 4);
      coordinates_[1][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[2][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[2][1] = QPair<int, int>(-1, 4);
      coordinates_[2][2] = QPair<int, int>(defoultValue, defoultValue);

      break;
    case figureL:
      coordinates_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[0][1] = QPair<int, int>(-3, 4);
      coordinates_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[1][1] = QPair<int, int>(-2, 4);
      coordinates_[1][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[2][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[2][1] = QPair<int, int>(-1, 4);
      coordinates_[2][2] = QPair<int, int>(-1, 5);

      break;
    case figureSqr:
      coordinates_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[0][1] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[1][1] = QPair<int, int>(-2, 4);
      coordinates_[1][2] = QPair<int, int>(-2, 5);

      coordinates_[2][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[2][1] = QPair<int, int>(-1, 4);
      coordinates_[2][2] = QPair<int, int>(-1, 5);

      break;
    case figureT:
      coordinates_[0][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[0][1] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[0][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[1][0] = QPair<int, int>(defoultValue, defoultValue);
      coordinates_[1][1] = QPair<int, int>(-2, 4);
      coordinates_[1][2] = QPair<int, int>(defoultValue, defoultValue);

      coordinates_[2][0] = QPair<int, int>(-1, 3);
      coordinates_[2][1] = QPair<int, int>(-1, 4);
      coordinates_[2][2] = QPair<int, int>(-1, 5);

      break;
  }


  switch (colorType) {
    case 1:
      kCellColor = QColor(82, 52, 235);
      break;
    case 2:
      kCellColor = QColor(52, 225, 235);
      break;
    case 3:
      kCellColor = QColor(235, 52, 146);
      break;
    case 4:
      kCellColor = QColor(168, 52, 235);
      break;
  }
}

void Figure::MoveFigure(QVector<QVector<QColor>> globalCoordinates, uint moveSide) {
  const uint left = 1;
  const uint right = 2;
  if (HasCollisions(globalCoordinates, moveSide)) {
    for (auto& row : coordinates_) {
      for (auto& cell : row) {
          if (cell == QPair<int, int>(0, 0)) continue;
          if (moveSide == left) { cell.second -= 1; }
          else { cell.second += 1; }
      }
    }
  }
}

bool Figure::StopMoveDown(QVector<QVector<QColor>> globalCoordinates) {
  if (HasCollisions(globalCoordinates, 3)) {
    for (auto& row : coordinates_) {
      for (auto& cell : row) {
          if (cell == QPair<int, int>(0, 0)) continue;
          cell.first += 1;
      }
    }
  } else return true;
  return false;
}

bool Figure::HasCollisions(QVector<QVector<QColor>> globalCoordinates, int side) {
  QPair<int, int> defaultValue(-100, -100);
  const QColor kCellStartColor = QColor(254, 217, 255);
  switch (side) {
    case(1):
      for (auto& row : coordinates_) {
        for (auto& cell : row) {
          if (cell == defaultValue) continue;
          if (cell.first < 0) continue;
          if (cell.second <= 0) { return false; }
          if (globalCoordinates[cell.first][cell.second - 1] != kCellStartColor)
            return false;
        }
      }
      return true;
      break;

      case(2):
        for (auto& row : coordinates_) {
          for (auto& cell : row) {
              if (cell == defaultValue) continue;
              if (cell.first < 0) continue;
              if (cell.second >= 9) { return false; }
              if (globalCoordinates[cell.first][cell.second + 1] != kCellStartColor)
                  return false;
          }
        }
        return true;
        break;

      case(3):
        for (auto& row : coordinates_) {
          for (auto& cell : row) {
              if (cell == defaultValue) continue;
              if (cell.first < 0) continue;
              if (cell.first == 19) { return false; }
              if ((globalCoordinates[cell.first + 1][cell.second] != kCellStartColor)) { return false; }
          }
        }
        return true;
        break;
    }
}

QColor Figure::GetColor() { return kCellColor; }

QVector<QVector<QPair<int, int>>> Figure::GetCoordinates() { return coordinates_; }


