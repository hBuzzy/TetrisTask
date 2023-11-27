#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>

class Figure final {

 public:
  explicit Figure();
  QVector<QVector<QPair<int, int>>> GetCoordinates();
  QColor GetColor();
  void RotateFigure(QVector<QVector<QColor>> coordinateGlobal);
  void MoveRight(QVector<QVector<QColor>> coordinateGlobal);
  void MoveLeft(QVector<QVector<QColor>> coordinateGlobal);
  bool MoveDown(QVector<QVector<QColor>> coordinateGlobal);

 protected:
  bool HasCollisions(QVector<QVector<QColor>>, int);

 private:
   QColor kCellColor;
   QVector<QVector<QPair<int, int>>> coordinates_;

};

#endif // FIGURE_H
