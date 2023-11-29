#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>

class Figure final {

 public:
  explicit Figure();
  QVector<QVector<QPair<int, int>>> GetCoordinates();
  QColor GetColor();
  void MoveFigure(QVector<QVector<QColor>> globalCoordinates, uint moveSide);
  bool StopMoveDown(QVector<QVector<QColor>> globalCoordinates);

 protected:
  bool HasCollisions(QVector<QVector<QColor>>, int);

 private:
   QColor kCellColor;
   QVector<QVector<QPair<int, int>>> coordinates_;

};

#endif // FIGURE_H
