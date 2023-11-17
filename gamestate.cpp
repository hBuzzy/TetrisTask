#include "gamestate.h"

GameState::GameState(QObject *parent): QObject(parent) {
    timer_ = new QTimer(this);
    connect(timer_, & QTimer::timeout, this, & GameState::updateGameState);
    timer_ -> start(1000);

    startNewGame();
}

int GameState::getScore() {
    return score_;
}

Figure GameState::getNextFigure() {
    return nextFigure_;
}

void GameState::startNewGame(){
    gameBoard_ = QVector<QVector<int>> (20, QVector<int> (10, 0));
    score_ = 0;
    nextFigure_.generate();
    spawnNewFigure();
}

QVector<QVector<int>> GameState::getGameBoard() {
    QVector<QVector<int>> updatedGameBoard = gameBoard_;
    for (int i = 0; i < currentFigure_.getHeight(); ++i) {
        for (int j = 0; j < currentFigure_.getWidth(); ++j) {
            if (currentFigure_.getShapeAt(j, i) == 1) {
                updatedGameBoard[currentRow_ + i][currentColumn_ + j] = 1;
            }
        }
    }
    return updatedGameBoard;
}

void GameState::moveLeft() {
    if (!isCollision(-1, 0)) {
        currentColumn_--;
        emit figureMoved();
    }
}

void GameState::moveRight() {
    if (!isCollision(1, 0)) {
        currentColumn_++;
        emit figureMoved();
    }
}

void GameState::moveDown() {
    if (!isCollision(0, 1)) {
        currentRow_++;
        emit figureMoved();
    } else {
        for (int i = 0; i < currentFigure_.getHeight(); ++i)
            for (int j = 0; j < currentFigure_.getWidth(); ++j)
                if (currentFigure_.getShapeAt(j, i) == 1)
                    gameBoard_[currentRow_ + i][currentColumn_ + j] = 1;

        clearLines();
        spawnNewFigure();
    }
}

void GameState::rotate() {
    Figure rotatedFigure = currentFigure_;
    rotatedFigure.rotate();

    if (isCollision(rotatedFigure, 0, 0)) {
        if (!isCollision(currentFigure_, 3, 0)) {
            while (isCollision(rotatedFigure, 0, 0)) {
                currentColumn_++;
            }
        } else if (!isCollision(currentFigure_, 3, 0)) {
            while (isCollision(rotatedFigure, 0, 0)) {
                currentColumn_++;
            }
        } else {
            return;
        }
    }

    currentFigure_ = rotatedFigure;
    emit figureMoved();
}

void GameState::updateGameState() {
    moveDown();
}

void GameState::spawnNewFigure() {

    currentFigure_ = nextFigure_;
    nextFigure_.generate();

    currentRow_ = 0;
    currentColumn_ = 2;

    if (isCollision(0, 0)) {
        startNewGame();
    }

    emit figureMoved();
}

bool GameState::isCollision(int xOffset, int yOffset) {
    return isCollision(currentFigure_, xOffset, yOffset);
}

bool GameState::isCollision(const Figure &rotatedFigure, int xOffset, int yOffset) {
    for (int i = 0; i < rotatedFigure.getHeight(); ++i)
        for (int j = 0; j < rotatedFigure.getWidth(); ++j)
            if (rotatedFigure.getShapeAt(j, i) == 1)
                if (currentColumn_ + j + xOffset < 0 || currentColumn_ + j + xOffset >= gameBoard_[0].size() ||
                    currentRow_ + i + yOffset >= gameBoard_.size() ||
                    gameBoard_[currentRow_ + i + yOffset][currentColumn_ + j + xOffset] == 1)
                    return true;

    return false;
}

void GameState::clearLines() {
    for (int i = gameBoard_.size() - 1; i >= 0; --i) {
        if (std::all_of(gameBoard_[i].begin(), gameBoard_[i].end(), [](int val) {
                return val == 1;
            })) {
            gameBoard_.remove(i);
            gameBoard_.prepend(QVector<int> (gameBoard_[0].size(), 0));
            emit lineCleared();
            i++;
            score_ += 4;
        }
    }
}
