#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "figure.h"

class GameState : public QObject
{
    Q_OBJECT
public:
    explicit GameState(QObject *parent = nullptr);
    QVector<QVector<int>> getGameBoard();
    Figure getNextFigure();
    int getScore();
    void startNewGame();

public slots:
    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();

private slots:
    void updateGameState();

signals:
    void figureMoved();
    void lineCleared();

private:
    void spawnNewFigure();
    bool isCollision(int xOffset, int yOffset);
    bool isCollision(const Figure& rotatedFigure, int xOffset, int yOffset);
    void clearLines();

    QVector<QVector<int>> gameBoard_;
    Figure currentFigure_;
    Figure nextFigure_;
    int currentRow_;
    int currentColumn_;
    QTimer* timer_;
    int score_;
};

#endif // GAMESTATE_H
