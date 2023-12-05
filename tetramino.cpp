#include "tetramino.h"

Tetramino::Tetramino(QObject *parent) : QObject(parent) {}

Tetramino::~Tetramino() {}

void Tetramino::SetRandomFigure() {
    int randNumb = 1 + rand() % 7;
    switch (randNumb) {
    case 1:
        currentFigure_ = {
            {1},
            {1},
            {1},
            {1}
        };
        colorFigure_ = Qt::cyan;
        break;
    case 2:
        currentFigure_ = {
            {0, 1},
            {0, 1},
            {1, 1}
        };
        colorFigure_ = Qt::magenta;
        break;
    case 3:
        currentFigure_ = {
            {1, 0},
            {1, 0},
            {1, 1}
        };
        colorFigure_ = Qt::blue;
        break;
    case 4:
        currentFigure_ = {
            {1, 1},
            {1, 1}
        };
        colorFigure_ = Qt::yellow;
        break;
    case 5:
        currentFigure_ = {
            {1, 1, 0},
            {0, 1, 1}
        };
        colorFigure_ = Qt::green;
        break;
    case 6:
        currentFigure_ = {
            {0, 1, 0},
            {1, 1, 1}
        };
        colorFigure_ = Qt::gray;
        break;
    case 7:
        currentFigure_ = {
            {0, 1, 1},
            {1, 1, 0}
        };
        colorFigure_ = Qt::red;
        break;
    default:
        break;
    }
}

QVector<QVector<int>> Tetramino::GetCurrentFigure() {
    SetRandomFigure();
    return currentFigure_;
}

QColor Tetramino::GetColorFigure() {
    return colorFigure_;
}
