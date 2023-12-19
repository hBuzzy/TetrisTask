#include "gamefield.h"

#include <QDebug>
#include <QPainter>
#include <QKeyEvent>

GameField::GameField(QWidget *parent) : QWidget{parent} {
  setFocusPolicy(Qt::StrongFocus);

  connect(this, &GameField::initialisationStarted, this, &GameField::SetCells,
          Qt::QueuedConnection);
  connect(this, &GameField::moveFigure, this, &GameField::refresh);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &GameField::moveDown);
  timer_ -> start(500);
  startNewGame();
  emit initialisationStarted();
}

uint GameField::GetRowsNumber() const { return rowsNumber_; }

void GameField::SetRowsNumber(uint rowsNumber) {
  if (rowsNumber_ == rowsNumber) return;
  rowsNumber_ = rowsNumber;
  emit rowsNumberChanged();
}

uint GameField::GetColumnsNumber() const { return columnsNumber_; }

void GameField::SetColumnNumber(uint columnsCount) {
  if (columnsNumber_ == columnsCount) return;
  columnsNumber_ = columnsCount;
  emit columnsNumberChanged();
}

void GameField::drawFigure(Figure figure, QPainter *painter, QPoint currentPosition) {

    for (int i = 0; i < figure.getFigure().size(); i++) {
        for (int j = 0; j < figure.getFigure()[i].size(); j++) {
            if (figure.getFigure()[j][i] == 1) {
                painter->fillRect(( j + currentPosition.x()) * blockSize_, (i + currentPosition.y()) * blockSize_, blockSize_, blockSize_, figure.getColor());
            }
        }
    }
}

void GameField::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    int nextLayerPositionX = 11;

    QPainter *painter = new QPainter(this);
    QVector<QVector<int>> gameField = gameField_;

    painter->fillRect(0, 0, gameField[0].size()*blockSize_, gameField.size()*blockSize_, Qt::white);
    for (int i = 0; i < gameField.size(); i++) {
        for (int j = 0; j < gameField[i].size(); j++) {
            if (gameField[i][j] == 1) {
                painter->fillRect(j * blockSize_, i * blockSize_, blockSize_, blockSize_, Qt::yellow);
            } else {
                painter->drawRect(j * blockSize_, i * blockSize_, blockSize_, blockSize_);
            }
        }
    }

    drawFigure(currentFigure_, painter, QPoint(currentXPosition_, currentYPosition_));
    drawFigure(nextFigure_, painter, QPoint(nextLayerPositionX, 0));
    painter->drawText(350, 125, QString("Score: %1").arg(score_));

}

void GameField::SetCells() {
  qDebug() << rowsNumber_ << " " << columnsNumber_ << "|";
}

void GameField::keyPressEvent(QKeyEvent *event) {
    switch( event->key()) {
        case Qt::Key_Left:
            moveFigureLeft();
            break;
        case Qt::Key_Right:
            moveFigureRight();
            break;
        case Qt::Key_Up:
            rotateFigure();
            break;
        case Qt::Key_Down:
            moveDown();
            break;
    }
}

void GameField::rotateFigure() {
    Figure rotatedFigure = currentFigure_;
    rotatedFigure.rotate();
        if (!isCollision(rotatedFigure, 0, 0)) {
          currentFigure_ = rotatedFigure;
          emit moveFigure();
      }
  }

void GameField::moveFigureRight() {
     if(!isCollision(currentFigure_, 1, 0)) {
        currentXPosition_++;
        emit moveFigure();
    }
}

void GameField::moveFigureLeft() {
    if(!isCollision(currentFigure_, -1, 0)) {
    currentXPosition_--;
    emit moveFigure();
    }
}

void GameField::moveDown() {
    if(!isCollision(currentFigure_, 0, 1)) {
    currentYPosition_++;
    emit moveFigure();
    }else {
        for (int i = 0; i < currentFigure_.getYPoints(); i++)
            for (int j = 0; j < currentFigure_.getXPoints(); j++)
                if (currentFigure_.getFigureAt(QPoint(i, j)) == 1)
                    gameField_[currentYPosition_ + i][currentXPosition_ + j] = 1;
        setNewFigure();
        clearLines();
    }
}

void GameField::refresh() {
    repaint();
}

bool GameField::isCollision(const Figure &movedFigure, int xOffset, int yOffset) {

    for (int i = 0; i <= movedFigure.getYPoints(); i++) {
        for (int j = 0; j < movedFigure.getXPoints(); j++) {
            if (movedFigure.getFigureAt(QPoint(i, j)) == 1) {
                if (currentXPosition_ + j + xOffset < 0 || currentXPosition_ + j + xOffset >= gameField_[0].size()  ||
                    currentYPosition_ + i + yOffset >= gameField_.size() ||
                    gameField_[currentYPosition_ + i + yOffset][currentXPosition_ + j + xOffset] == 1)
                    return true;
            }
        }
    }
    return false;
}

void GameField::setNewFigure() {
    currentFigure_ = nextFigure_;
    nextFigure_.generateFigure();
    nextFigure_.generateColor();
    currentYPosition_ = 0;
    currentXPosition_ = 3;
    emit moveFigure();
        if(isCollision(currentFigure_, 0, 0)) {
            startNewGame();
    };
}

void GameField::startNewGame() {
    int xPoints = 10;
    int yPoints = 20;
    gameField_ = QVector<QVector<int>> (yPoints, QVector<int> (xPoints, 0));
    currentFigure_.generateFigure();
    currentFigure_.generateColor();
    nextFigure_.generateFigure();
    nextFigure_.generateColor();
    currentYPosition_ = 0;
    currentXPosition_ = 3;
    score_ = 0;
}

void GameField::clearLines() {
    for (int i = 0; i <= gameField_.size() - 1; i++) {
        if (std::all_of(gameField_[i].begin(), gameField_[i].end(), [](int val) { return val == 1; })) {
            gameField_.remove(i);
            gameField_.prepend(QVector<int> (gameField_[0].size(), 0));
            changeScore(100);
            i--;
        }
    }
}

  void GameField::changeScore(int points){
      score_=score_ + points;
  }
