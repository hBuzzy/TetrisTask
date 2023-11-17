#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPushButton>
#include <QMainWindow>
#include "gamestate.h"

class GameField : public QWidget {
    Q_OBJECT
public:
    explicit GameField(QWidget *parent = nullptr);

    Q_PROPERTY(int rowsNumber READ getRowsNumber WRITE setRowsNumber NOTIFY
                   rowsNumberChanged FINAL)
    Q_PROPERTY(int columnsNumber READ getColumnsNumber WRITE setColumnNumber
                   NOTIFY columnsNumberChanged FINAL)

public slots:
    void updateUI();
    void paintEvent(QPaintEvent *event);
    void startNewGame();

signals:
    void rowsNumberChanged();
    void columnsNumberChanged();
    void leftKeyPressed();
    void downKeyPressed();
    void rightKeyPressed();
    void rotateKeyPressed();

public:
    int getRowsNumber() const;
    void setRowsNumber(int newRowsNumber);

    int getColumnsNumber() const;
    void setColumnNumber(int newColumnsCount);

private:
    int rowsNumber_ = 0;
    int columnsNumber_ = 0;
    GameState *gameState_;
};

#endif // GAMEFIELD_H
