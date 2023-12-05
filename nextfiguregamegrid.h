#ifndef NEXTFIGUREGAMEGRID_H
#define NEXTFIGUREGAMEGRID_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QColor>
#include "tetramino.h"

class NextFigureGameGrid : public QWidget {
    Q_OBJECT
public:
    explicit NextFigureGameGrid(QWidget *parent = nullptr);
    void SetNextFigure();

    QVector<QVector<int>> GetNextFigure();
signals:
    void nextFigureChanged();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QVector<int>> nextFigure_;
    int blockSize_;

    Tetramino *tetr_;
};
#endif // NEXTFIGUREGAMEGRID_H
