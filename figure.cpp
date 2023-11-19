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

int Figure::GetNumberOfCells() const { return kNumberOfCells_; };

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

int Figure::GetMinX() const {
  int min = coordinates_[0][0];
  for (int i = 1; i < kNumberOfCells_; ++i) {
    min = qMin(min, coordinates_[i][0]);
  }
  return min;
}

int Figure::GetMaxX() const {
  int max = coordinates_[0][0];
  for (int i = 1; i < kNumberOfCells_; ++i) {
    max = qMax(max, coordinates_[i][0]);
  };
  return max;
}

int Figure::GetMinY() const {
  int min = coordinates_[0][1];
  for (int i = 1; i < kNumberOfCells_; ++i) {
    min = qMin(min, coordinates_[i][1]);
  };
  return min;
}

int Figure::GetMaxY() const {
  int max = coordinates_[0][1];
  for (int i = 1; i < kNumberOfCells_; ++i) {
    max = qMax(max, coordinates_[i][1]);
  }
  return max;
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
