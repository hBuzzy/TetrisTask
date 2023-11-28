#include "gamefield.h"

#include <QDebug>
#include <QPainter>
#include <QtAlgorithms>
#include <QTimer>
#include <QVector>

GameField::GameField(QWidget *parent) : QWidget{parent} {
    setStyleSheet("border: 4px solid black;");

  emit InitialisationStarted();
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
    if (column >= 0 && column + currentFigure_[0].size() <= columnsNumber_ && GameGrid_[row][column] != 1) {
        currentFigureRow_ = row;
        currentFigureColumn_ = column;
    }
    update();
}

void GameField::spawnNextFigure() {

        int randNumb = 1 + rand() % 7;
        //int randNumb = 4;

        switch (randNumb) {
        case 1:
            currentFigure_ = {
                {1},
                {1},
                {1},
                {1}
            };
            colorFigure = Qt::cyan;
            break;
        case 2:
            currentFigure_ = {
                {0, 1},
                {0, 1},
                {1, 1}
            };
            colorFigure = Qt::magenta;
            break;
        case 3:
            currentFigure_ = {
                {1, 0},
                {1, 0},
                {1, 1}
            };
            colorFigure = Qt::blue;
            break;
        case 4:
            currentFigure_ = {
                {1, 1},
                {1, 1}
            };
            colorFigure = Qt::yellow;
            break;
        case 5:
            currentFigure_ = {
                {1, 1, 0},
                {0, 1, 1}
            };
            colorFigure = Qt::green;
            break;
        case 6:
            currentFigure_ = {
                {0, 1, 0},
                {1, 1, 1}
            };
            colorFigure = Qt::gray;
            break;
        case 7:
            currentFigure_ = {
                {0, 1, 1},
                {1, 1, 0}
            };
            colorFigure = Qt::red;
            break;
        default:
            break;
        }

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


void GameField::updateGameGrid() {
    for (int i = 0; i < currentFigure_.size(); ++i) {
        for (int j = 0; j < currentFigure_[0].size(); ++j) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i;
                int y = currentFigureColumn_ + j;

                CheckLine();
                //CheckColumn();
                GameGrid_[x][y] = 1;
            }
        }
    }
}

bool GameField::CheckCollision() {
    for (int i = 0; i < currentFigure_.size(); i++) {
        for (int j = 0; j < currentFigure_[0].size(); j++) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i + 1;
                int y = currentFigureColumn_ + j;

                if (x < rowsNumber_ && GameGrid_[x][y] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool GameField::CheckCollisionMoveLeft() {
    for (int i = 0; i < currentFigure_.size(); i++) {
        for (int j = 0; j < currentFigure_[0].size(); j++) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i;
                int y = currentFigureColumn_ + j - 1;

                if (y < 0 || (x < rowsNumber_ && GameGrid_[x][y] == 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameField::CheckCollisionMoveRight() {
    for (int i = 0; i < currentFigure_.size(); i++) {
        for (int j = 0; j < currentFigure_[0].size(); j++) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i;
                int y = currentFigureColumn_ + j + 1;

                if (y >= columnsNumber_ || (x < rowsNumber_ && GameGrid_[x][y] == 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameField::CheckCollisionRotate() {

    for (int i = 0; i < currentFigure_.size(); i++) {
        for (int j = 0; j < currentFigure_[0].size(); j++) {
            if (currentFigure_[i][j] == 1) {
                int x = currentFigureRow_ + i;
                int y = currentFigureColumn_ + j;

                if (x >= rowsNumber_ || y < 0 || y >= columnsNumber_ || GameGrid_[x][y] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameField::CheckFullLine(int row) {
    for(int j = 0; j < columnsNumber_; j++) {
        if(GameGrid_[row][j] != 1) {
            return false;
        }
    }
    return true;
}

bool GameField::CheckFullColumn(int column) {
    for (int i = 0; i < rowsNumber_; i++) {
        if (GameGrid_[i][column] != 1) {
            return false;
        }
    }
    return true;
}

void GameField::RemoveFullLine(int row) {
    for (int j = 0; j < columnsNumber_; j++) {
        GameGrid_[row][j] = 0;
    }

    for(int k = row;k > 0;k--) {
        for(int j = 0;j < columnsNumber_;j++) {
            GameGrid_[k][j] = GameGrid_[k-1][j];
        }
    }

}

int GameField::GetWinPoints() {
    return WinPoints;
}

// Основная функция
void GameField::CheckLine() {
    for(int i = rowsNumber_ - 1; i >= 0; i--) {
        if (CheckFullLine(i)) {
            RemoveFullLine(i);
            WinPoints += 100;
            emit ChangeWinPoints();
            qDebug() << "Ваши очки = " << WinPoints;
        }
    }
}

void GameField::CheckColumn() {
    for(int j = columnsNumber_ - 1; j >= 0; j--) {
        if (CheckFullColumn(j)) {
            //emit GameOver();
            isGameOver = true;
            qDebug() << "Заполнен столбец" << j;
        }
    }
}

void GameField::moveFigure() {
    CheckColumn();

    if(!isGameOver) {
        // Проверка, не касается ли текущая фигура нижней границы поля
        if (currentFigureRow_ + currentFigure_.size() < rowsNumber_) {
            bool isHaveCollision = CheckCollision();

            if (isHaveCollision) {
                // Если столкнулись с другой фигурой, сохраняем текущую фигуру и спавним новую
                qDebug() << "коснулись другую фигуру";
                updateGameGrid();
                spawnNextFigure();
                SetFigurePosition(0, columnsNumber_ / 2);
                CheckLine();
            } else {
                SetFigurePosition(currentFigureRow_ + 1, currentFigureColumn_);
            }
        } else {
            // Если достигли нижней границы, сохраняем текущую фигуру и спавним новую
            qDebug() << "Достигли нижнюю границу";
            updateGameGrid();
            spawnNextFigure();
            SetFigurePosition(0, columnsNumber_ / 2);
            CheckLine();
        }

        //qDebug() << "currentFigureRow_ = " << currentFigureRow_;

        update();
    } else {
        qDebug() << "Вы проиграли";
        isGameOver = false;
        emit GameOver();

    }

}


uint GameField::GetCurrentFigureRow() {
    return currentFigureRow_;
}

uint GameField::GetCurrentFigureColumn() {
    return currentFigureColumn_;
}

void GameField::clearGameGrid() {
    GameGrid_ = QVector<QVector<int>>(rowsNumber_, QVector<int>(columnsNumber_, 0));

    currentFigure_ = QVector<QVector<int>>();
    currentFigureRow_ = 0;
    currentFigureColumn_ = 0;

    qDebug() << "Поле очищено";

    update();
}

void GameField::SetWinPoints(int points) {
    WinPoints = points;
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
                int x = (currentFigureColumn_ + j) * blockSize;
                int y = (currentFigureRow_ + i) * blockSize;
                painter.fillRect(x, y, blockSize, blockSize, colorFigure);
            }
        }
    }

    for (int i = 0; i < rowsNumber_;i++) {
        for (int j = 0; j < columnsNumber_;j++) {
            if (GameGrid_[i][j] == 1) {
                int x = j * blockSize;
                int y = i * blockSize;               
                painter.fillRect(x, y, blockSize, blockSize, Qt::blue);
                painter.drawRect(j * blockSize, i * blockSize, blockSize, blockSize);
            } else {
                painter.drawRect(j * blockSize, i * blockSize, blockSize, blockSize);
            }
        }
    }
    update();
}

