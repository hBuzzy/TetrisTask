#include "nextfiguregamegrid.h"

NextFigureGameGrid::NextFigureGameGrid(QWidget *parent) : QWidget(parent), blockSize_(30) {
    setFixedSize(120, 120);

    tetramino_ = new Tetramino(this);

    nextFigure_ = QVector<QVector<int>>();
}

void NextFigureGameGrid::SetNextFigure() {
    nextFigure_ = tetramino_->GetCurrentFigure();
    emit NextFigureChanged();
    update();
}
QVector<QVector<int>> NextFigureGameGrid::GetNextFigure() {
    return nextFigure_;
}

void NextFigureGameGrid::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.fillRect(rect(), Qt::white);

    for (int i = 0; i < nextFigure_.size(); ++i) {
        for (int j = 0; j < nextFigure_[0].size(); ++j) {
            if (nextFigure_[i][j] == 1) {
                int x = j * blockSize_;
                int y = i * blockSize_;
                painter.fillRect(x, y, blockSize_, blockSize_, Qt::blue);
                painter.drawRect(x, y, blockSize_, blockSize_);
            } else {
                painter.drawRect(j * blockSize_, i * blockSize_, blockSize_, blockSize_);
            }
        }
    }
    update();
}
