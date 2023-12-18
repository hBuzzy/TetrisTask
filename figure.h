#ifndef FIGURE_H
#define FIGURE_H

#include <QVector>;
#include <QObject>;
#include <QColor>

class Figure
{
public:
    Figure();

    void rotateFigure();
    void generateFigure();
    int getXPoints() const;
    int getYPoints() const;
    int getFigureAt(int yPoints, int xPoints) const;
    QVector<QVector<int>> getFigure();
    void generateColor();
    QColor getColor();

private:
    QVector<QVector<int>> figure_;
    QColor color_;
};

#endif // FIGURE_H
