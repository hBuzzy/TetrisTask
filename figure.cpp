#include "figure.h"

Figure::Figure(QObject *parent): QObject(parent) {}

Figure::Figure(const Figure &other): QObject(other.parent()) {
    copyFrom(other);
}

Figure &Figure::operator = (const Figure & other) {
    if (this != &other) {
        setParent(other.parent());
        copyFrom(other);
    }
    return *this;
}

void Figure::copyFrom(const Figure & other) {
    shape_ = other.shape_;
}

int Figure::getHeight() const {
    return shape_.size();
}

int Figure::getWidth() const {
    return shape_.isEmpty() ? 0 : shape_[0].size();
}

int Figure::getShapeAt(int row, int col) const {
    if (row >= 0 && row < getHeight() && col >= 0 && col < getWidth()) return shape_[row][col];
    return 0;
}

void Figure::generate() {
    srand(static_cast<unsigned>(time(0)));
    int randomIndex = rand() % lenght;

    switch (randomIndex) {
        case 0:
            shape_ = I_;
            break;
        case 1:
            shape_ = J_;
            break;
        case 2:
            shape_ = L_;
            break;
        case 3:
            shape_ = O_;
            break;
        case 4:
            shape_ = S_;
            break;
        case 5:
            shape_ = T_;
            break;
        case 6:
            shape_ = Z_;
            break;
        default:
            shape_ = {};
    }
}

void Figure::rotate() {
    QVector<QVector<int>> rotated(getWidth(), QVector<int> (getHeight()));

    for (int i = 0; i < getHeight(); ++i) {
        for (int j = 0; j < getWidth(); ++j){
            rotated[j][getHeight() - 1 - i] = shape_[i][j];
        }
    }

    shape_ = rotated;
}

const QVector<QVector<int>> Figure::I_ = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const QVector<QVector<int>> Figure::J_ = {
    {0, 0, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const QVector<QVector<int>> Figure::L_ = {
    {1, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const QVector<QVector<int>> Figure::O_ = {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const QVector<QVector<int>> Figure::S_ = {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const QVector<QVector<int>> Figure::T_ = {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const QVector<QVector<int>> Figure::Z_ = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
