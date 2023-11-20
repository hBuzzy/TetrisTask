#ifndef FIGURE_H
#define FIGURE_H

#include <QColor>

enum Shapes { NoShape, I1, I2, I3, I4, Square, LShape };

class Figure {
 public:
  Figure();

  void RandomizeFigure();

  int GetMinX() const;
  int GetMaxX() const;
  int GetMinY() const;
  int GetMaxY() const;
  int GetExtreme(bool isX, bool isMax) const;

  int GetX(int index) const;
  int GetY(int index) const;

  int GetNumberCells() const;

  QColor GetColor() const;

  Shapes GetShape();
  void SetShape(Shapes shape);

  Figure Rotate(bool clockwise);
  Figure& operator=(const Figure& other) noexcept;

  static const int kNumberOfShapes_ = 7;

  const QColor KColors_[kNumberOfShapes_] = {
      QColor(0, 0, 0),       QColor(255, 179, 186), QColor(255, 223, 186),
      QColor(255, 255, 186), QColor(186, 255, 201), QColor(186, 225, 255),
      QColor(224, 187, 228)};

 private:
  void SetColor(const int numberOfShape);
  void SetX(int index, int x);
  void SetY(int index, int y);

 private:
  Shapes shape_;
  QColor color_;

  static const int kNumberOfCells_ = 4;
  static const int kNumberOfCoordinates_ = 2;

  int coordinates_[kNumberOfCells_][kNumberOfCoordinates_];
};

#endif  // FIGURE_H
