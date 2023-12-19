#include "figure.h"
#include <time.h>

Figure::Figure() {}

void Figure::generateFigure() {
    QVector<QVector<QVector<int>>> figures;
    figures.append( { {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} } );
    figures.append( { {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0} } );
    figures.append( { {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} } );
    figures.append( { {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} } );
    figures.append( { {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} } );
    figures.append( { {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } );
    srand(time(NULL));
    int index = rand() % figures.size();
    figure_ = figures[index];
}

int Figure::getXPoints() const {
    return figure_.size();
}

int Figure::getYPoints() const {
    return figure_[0].size();
}

int Figure::getFigureAt(QPoint point) const {
    if (point.y() >= 0 && point.y() < getYPoints() && point.x() >= 0 && point.x() < getXPoints())
        return figure_[point.y()][point.x()];
    return 0;
}
QVector<QVector<int>> Figure::getFigure() {
    return figure_;
}
void Figure::rotate() {
    int xPoints = getXPoints();
    int yPoints = getYPoints();
    QVector<QVector<int>> rotated(xPoints, QVector<int> (yPoints));

    for (int i = 0; i < yPoints; i++) {
        for (int j = 0; j < xPoints; j++) {
            rotated[j][yPoints - 1 - i] = figure_[i][j];
        }
    }
    figure_ = rotated;
}
void Figure::generateColor() {
    QVector<QColor> colors;
    colors.append( QColor(255, 135, 50));
    colors.append( QColor(168, 50, 168));
    colors.append( QColor(0, 255, 0));
    colors.append( QColor(0, 255, 255));
    colors.append( QColor(127, 0, 255));

    srand(time(NULL));
    int index = rand() % colors.size();
    color_ = colors[index];
}

QColor Figure::getColor() {
    return color_;
}
