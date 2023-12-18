#include "figure.h"
#include <time.h>

Figure::Figure() {}

void Figure::generateFigure(){
    QVector<QVector<QVector<int>>> arrayOfFigures;
    arrayOfFigures.append( { {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} } );
    arrayOfFigures.append( { {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0} } );
    arrayOfFigures.append( { {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} } );
    arrayOfFigures.append( { {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} } );
    arrayOfFigures.append( { {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} } );
    arrayOfFigures.append( { {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } );
    srand(time(NULL));
    int type = rand() % arrayOfFigures.size();
    figure_ = arrayOfFigures[type];
}
int Figure::getXPoints() const{
    return figure_.size();
}

int Figure::getYPoints() const{
    return figure_[0].size();
}

int Figure::getFigureAt(int yPoints, int xPoints) const {
    if (yPoints >= 0 && yPoints < getYPoints() && xPoints >= 0 && xPoints < getXPoints()) return figure_[yPoints][xPoints];
    return 0;
}
QVector<QVector<int>> Figure::getFigure() {
    return figure_;
}
void Figure::rotateFigure() {
    QVector<QVector<int>> rotated(getXPoints(), QVector<int> (getYPoints()));

    for (int i = 0; i < getYPoints(); i++) {
        for (int j = 0; j < getXPoints(); j++){
            rotated[j][getYPoints() - 1 - i] = figure_[i][j];
        }
    }
    figure_ = rotated;
}
void Figure::generateColor() {
    QVector<QColor> arrayOfColors;
    arrayOfColors.append( QColor(255, 135, 50) );
    arrayOfColors.append( QColor(168, 50, 168) );
    arrayOfColors.append( QColor(0, 255, 0) );
    arrayOfColors.append( QColor(0, 255, 255) );
    arrayOfColors.append( QColor(127, 0, 255) );

    srand(time(NULL));
    int type = rand() % arrayOfColors.size();
    color_ = arrayOfColors[type];
}

QColor Figure::getColor() {
return color_;
}
