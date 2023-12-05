#ifndef TETRAMINO_H
#define TETRAMINO_H

#include <QVector>
#include <QObject>
#include <QColor>

class Tetramino : public QObject {
    Q_OBJECT
public:
    explicit Tetramino(QObject *parent = nullptr);
    ~Tetramino();
    void SetRandomFigure();

    QVector<QVector<int>> GetCurrentFigure();
    QColor GetColorFigure();

private:
    QVector<QVector<int>> currentFigure_;
    QColor colorFigure_;
};
#endif // TETRAMINO_H
