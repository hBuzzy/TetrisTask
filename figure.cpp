#include "figure.h"

#include <QDebug>
#include <QList>
#include <QRandomGenerator>

Figure::Figure() { SetShape(NoShape); }

void Figure::RandomizeFigure() {
  const int numberOfShape =
      QRandomGenerator::global()->bounded(kNumberOfShapes_ - 1) + 1;
  SetShape(Shapes(numberOfShape));
  SetColor(numberOfShape);
  qDebug() << "Shape: " << numberOfShape << " Color: " << color_;
}

int Figure::GetX(int index) const { return coordinates_[index][0]; }

int Figure::GetY(int index) const { return coordinates_[index][1]; }

int Figure::GetNumberCells() const { return kNumberOfCells_; };

void Figure::SetShape(Shapes shape) {
  const int coordinateTable[kNumberOfShapes_][kNumberOfCells_]
                           [kNumberOfCoordinates_] = {
                               {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
                               {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
                               {{0, 0}, {0, -1}, {0, 0}, {0, 0}},
                               {{0, 0}, {0, -1}, {0, 1}, {0, 0}},
                               {{0, 0}, {0, -1}, {0, 1}, {0, 2}},
                               {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
                               {{-1, -1}, {0, -1}, {0, 0}, {0, 1}}};

  for (int i = 0; i < kNumberOfCells_; i++) {
    for (int j = 0; j < kNumberOfCoordinates_; ++j)
      coordinates_[i][j] = coordinateTable[shape][i][j];
  }

  shape_ = shape;
}

Shapes Figure::GetShape() { return shape_; }

void Figure::SetColor(const int numberOfShape) {
  color_ = KColors_[numberOfShape];
}

void Figure::SetX(int index, int x) { coordinates_[index][0] = x; }

void Figure::SetY(int index, int y) { coordinates_[index][1] = y; }

int Figure::GetMinX() const { return GetExtreme(true, false); }

int Figure::GetMaxX() const { return GetExtreme(true, true); }

int Figure::GetMinY() const { return GetExtreme(false, false); }

int Figure::GetMaxY() const { return GetExtreme(false, true); }

int Figure::GetExtreme(bool isX, bool isMax) const {
  int extreme = coordinates_[0][isX ? 0 : 1];
  for (int i = 1; i < kNumberOfCells_; ++i) {
    int currentValue = coordinates_[i][isX ? 0 : 1];
    extreme = isMax ? qMax(extreme, currentValue) : qMin(extreme, currentValue);
  }
  return extreme;
}

QColor Figure::GetColor() const { return color_; }

Figure& Figure::operator=(const Figure& other) noexcept {
  if (this != &other) {
    for (int i = 0; i < kNumberOfCells_; ++i) {
      for (int j = 0; j < kNumberOfCoordinates_; ++j) {
        coordinates_[i][j] = other.coordinates_[i][j];
      }
    }
    shape_ = other.shape_;
    color_ = other.color_;
  }
  return *this;
}

Figure Figure::Rotate(bool clockwise) {
  if (shape_ == Square) return *this;

  Figure resultFigure;
  resultFigure = *this;

  for (int i = 0; i < 4; ++i) {
    int newX = clockwise ? GetY(i) : -GetY(i);
    int newY = clockwise ? -GetX(i) : GetX(i);

    resultFigure.SetX(i, newX);
    resultFigure.SetY(i, newY);
  }

  return resultFigure;
}
