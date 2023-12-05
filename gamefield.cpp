#include "gamefield.h"

#include <QDebug>
#include <QPainter>
#include <QtAlgorithms>
#include <QTimer>
#include <QVector>

GameField::GameField(QWidget *parent) : QWidget(parent) {
    setStyleSheet("border: 4px solid black;");

    SetColumnNumber(20);
    SetRowsNumber(24);

    gameGrid_ = QVector<QVector<int>>(rowsNumber_, QVector<int>(columnsNumber_, 0));

    currentFigureRow_ = 0;
    currentFigureColumn_ = 0;
}

void GameField::SetNextFigureGrid(QVector<QVector<int>> fig) {
    nextFigure_ = fig;
}

uint GameField::GetRowsNumber() const { return rowsNumber_; }

void GameField::SetRowsNumber(uint rowsNumber) {
    if (rowsNumber_ == rowsNumber) return;

    rowsNumber_ = rowsNumber;
    emit RowsNumberChanged();
    update();
}

uint GameField::GetColumnsNumber() const { return columnsNumber_; }

void GameField::SetColumnNumber(uint columnsCount) {
    if (columnsNumber_ == columnsCount) return;

    columnsNumber_ = columnsCount;
    emit ColumnsNumberChanged();
    update();
}

void GameField::SetFigurePosition(int row, int column) {
    if (column >= 0 && column + currentFigure_[0].size() <= columnsNumber_ && gameGrid_[row][column] != 1) {
        currentFigureRow_ = row;
        currentFigureColumn_ = column;
    }
    update();
}

void GameField::SpawnNextFigure() {
    currentFigure_ = nextFigure_;
    figureColor_ = Qt::blue;
    emit FigureSpawned();
}

QVector<QVector<int>> GameField::GetCurrentFigure() {
    return currentFigure_;
}

QVector<QVector<int>> GameField::GetRotateCurrentFigure() {
    QVector<QVector<int>> currFigure = GetCurrentFigure();
    QVector<QVector<int>> RotateFigure;

    RotateFigure.resize(currFigure[0].size());

    for (int i = 0; i < RotateFigure.size(); ++i) {
        RotateFigure[i].resize(currFigure.size());
    }


    for(int i = 0; i < currFigure.size(); i++) {
        for (int j = 0; j < currFigure[0].size(); j++) {
            RotateFigure[j][currFigure.size() - i - 1] = currFigure[i][j];
        }
    }

    currentFigure_ = RotateFigure;

    emit CurrentFigureChanged();
    return currentFigure_;
}

void GameField::UpdateGameGrid() {
    for (int i = 0; i < currentFigure_.size(); ++i) {
        for (int j = 0; j < currentFigure_[0].size(); ++j) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i;
                int y = currentFigureColumn_ + j;
                CheckingLine();
                gameGrid_[x][y] = 1;
            }
        }
    }
}

bool GameField::HasCollisionMove(int x0, int y0) {
    for (int i = 0; i < currentFigure_.size(); i++) {
        for (int j = 0; j < currentFigure_[0].size(); j++) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i + x0;
                int y = currentFigureColumn_ + j + y0;

                if (x < 0 || x >= rowsNumber_ || y < 0 || y >= columnsNumber_ || gameGrid_[x][y] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameField::HasCollisionRotation() {
    for (int i = 0; i < currentFigure_.size(); i++) {
        for (int j = 0; j < currentFigure_[0].size(); j++) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i;
                int y = currentFigureColumn_ + j;

                if (x >= rowsNumber_ || y < 0 || y >= columnsNumber_ || gameGrid_[x][y] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameField::HasFullLine(int row) {
    for(int j = 0; j < columnsNumber_; j++) {
        if(gameGrid_[row][j] != 1) {
            return false;
        }
    }
    return true;
}

void GameField::RemoveFullLine(int row) {
    for (int j = 0; j < columnsNumber_; j++) {
        gameGrid_[row][j] = 0;
    }

    for(int k = row;k > 0;k--) {
        for(int j = 0;j < columnsNumber_; j++) {
            gameGrid_[k][j] = gameGrid_[k-1][j];
        }
    }
}

int GameField::GetWinPoints() {
    return winPoints_;
}

void GameField::CheckingLine() {
    for(int i = rowsNumber_ - 1; i >= 0; i--) {
        if (HasFullLine(i)) {
            RemoveFullLine(i);
            winPoints_ += 100;
            emit ChangeWinPoints();
        }
    }
}

void GameField::MoveFigure() {
    GetGameOver();
    int CenterPointX = 2;

    if(!isGameOver_) {
        if (currentFigureRow_ + currentFigure_.size() < rowsNumber_) {
            bool HasCollision = HasCollisionMove(1, 0);


            if (HasCollision) {
                UpdateGameGrid();
                SpawnNextFigure();
                SetFigurePosition(0, columnsNumber_ / CenterPointX);
                CheckingLine();
            } else {
                SetFigurePosition(currentFigureRow_ + 1, currentFigureColumn_);
            }
        } else {
            UpdateGameGrid();
            SpawnNextFigure();
            SetFigurePosition(0, columnsNumber_ / CenterPointX);
            CheckingLine();
        }
        update();
    } else {
        isGameOver_ = false;
        emit GameOver();
    }
}

void GameField::GetGameOver() {
    for (int i = 0; i < columnsNumber_; i++) {
        if (gameGrid_[0][i] != 0) {
            isGameOver_ = true;
        }
    }
}

uint GameField::GetCurrentFigureRow() {
    return currentFigureRow_;
}

uint GameField::GetCurrentFigureColumn() {
    return currentFigureColumn_;
}

void GameField::ClearGameGrid() {
    gameGrid_ = QVector<QVector<int>>(rowsNumber_, QVector<int>(columnsNumber_, 0));

    currentFigure_ = QVector<QVector<int>>();
    currentFigureRow_ = 0;
    currentFigureColumn_ = 0;

    update();
}

void GameField::SetWinPoints(int points) {
    winPoints_ = points;
    emit ChangeWinPoints();
}

void GameField::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.fillRect(rect(), Qt::white);

    for (int i = 0; i < currentFigure_.size(); ++i) {
        for (int j = 0; j < currentFigure_[0].size(); ++j) {
            if (currentFigure_[i][j] == 1) {
                int x = (currentFigureColumn_ + j) * blockSize_;
                int y = (currentFigureRow_ + i) * blockSize_;
                painter.fillRect(x, y, blockSize_, blockSize_, figureColor_);
            }
        }
    }

    for (int i = 0; i < rowsNumber_; i++) {
        for (int j = 0; j < columnsNumber_; j++) {
            if (gameGrid_[i][j] == 1) {
                int x = j * blockSize_;
                int y = i * blockSize_;
                painter.fillRect(x, y, blockSize_, blockSize_, Qt::blue);
                painter.drawRect(j * blockSize_, i * blockSize_, blockSize_, blockSize_);
            } else {
                painter.drawRect(j * blockSize_, i * blockSize_, blockSize_, blockSize_);
            }
        }
    }
    update();
}
