#include "tetrisgrid.h"
#include <QPainter>
#include <QKeyEvent>
#include <QBrush>
#include <QMessageBox>

TetrisGrid::TetrisGrid(QWidget *parent)
    : QWidget(parent), mRows_(24), mColumns_(10), timer_(new QTimer(this)), score_(0) {
    mGrid_.resize(mRows_, QVector<int>(mColumns_, 0));
    setFocusPolicy(Qt::StrongFocus);
    connect(timer_, &QTimer::timeout, this, &TetrisGrid::dropShape);
    timer_->setInterval(speed_);
    startGame();
}

void TetrisGrid::addCurrentShapeToGridNew() {
    for (int i = 0; i < currentShape_.size(); ++i) {
        for (int j = 0; j < currentShape_[i].size(); ++j) {
            if (currentShape_[i][j] == 1) {
                int gridRow = shapeRow_ + i;
                int gridColumn = shapeColumn_ + j;
                if (gridRow >= 0 && gridRow < mRows_ && gridColumn >= 0 && gridColumn < mColumns_) {
                    if (gridRow <= 4) {
                        emit gameOver();
                    }
                    else
                    mGrid_[gridRow][gridColumn] = 1;
                }
            }
        }
    }
}

void TetrisGrid::setupGame() {
    mGrid_.clear();
    mGrid_.resize(mRows_, QVector<int>(mColumns_, 0));
    score_ = 0;
    updateScore(score_);
    generateNewShape();
}

void TetrisGrid::speedFigure(){
    timer_->setInterval(100);
    timer_->start();
}

int TetrisGrid::rows() const {
    return mRows_;
}

int TetrisGrid::columns() const {
    return mColumns_;
}

void TetrisGrid::setRows(int rows) {
    if (mRows_ != rows) {
        mRows_ = rows;
        mGrid_.resize(mRows_);
        for (auto &row : mGrid_) {
            row.resize(mColumns_);
        }
        emit rowsChanged(mRows_);
        update();
    }
}

void TetrisGrid::setColumns(int columns) {
    if (mColumns_ != columns) {
        mColumns_ = columns;
        for (auto &row_ : mGrid_) {
            row_.resize(mColumns_);
        }
        emit columnsChanged(mColumns_);
        update();
    }
}

void TetrisGrid::startGame() {
    setupGame();
    timer_->start();
}

void TetrisGrid::moveShapeLeft() {
    if (shapeColumn_ > 0) {
        --shapeColumn_;
        if (!isValidMove(currentShape_, shapeRow_, shapeColumn_)) {
            ++shapeColumn_;
        }
        update();
    }
}

void TetrisGrid::moveShapeRight() {
    if (shapeColumn_ + currentShape_[0].size() < mColumns_) {
        ++shapeColumn_;
        if (!isValidMove(currentShape_, shapeRow_, shapeColumn_)) {
            --shapeColumn_;
        }
        update();
    }
}

bool TetrisGrid::isValidMove(const QVector<QVector<int>> &shape, int rows, int columns) {
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] == 1) {
                int gridRow = rows + i;
                int gridColumn = columns + j;
                if (gridRow < 0 || gridRow >= mRows_ || gridColumn < 0 || gridColumn >= mColumns_) {
                    return false;
                }
                if (mGrid_[gridRow][gridColumn] == 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

void TetrisGrid::rotateShape()
{
    QVector<QVector<int>> rotatedShape;
    QVector<int> currentRow;
    for (int j = currentShape_[0].size() - 1; j >= 0; --j) {
        currentRow.clear();
        for (int i = 0; i < currentShape_.size(); ++i) {
            currentRow.append(currentShape_[i][j]);
        }
        rotatedShape.append(currentRow);
    }
    if (isValidMove(rotatedShape, shapeRow_, shapeColumn_)) {
        currentShape_ = rotatedShape;
        update();
    }
}

void TetrisGrid::dropShape() {
        if (shapeRow_ + currentShape_.size() <= mRows_) {
            ++shapeRow_;
            if (isCollision()) {
                --shapeRow_;
                addCurrentShapeToGridNew();
                generateNewShape();
                checkLines();
            }
            update();
        }
}

void TetrisGrid::updateScore(int newScore) {
    score_ = newScore;
    emit scoreChanged(score_);
}

void TetrisGrid::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    int cellSize = qMin(width() / mColumns_, height() / mRows_);
    drawGrid(painter, cellSize);
    drawShape(painter, cellSize);
    drawNextShape(painter, cellSize);
}

void TetrisGrid::drawGrid(QPainter &painter, int cellSize) {
    for (int row = 0; row < mRows_; ++row) {
        for (int column = 0; column < mColumns_; ++column) {
            if (mGrid_[row][column] == 1) {
                QColor color = generateRandomColor();
                painter.fillRect(column * cellSize, row * cellSize, cellSize, cellSize, color);
            } else {
                painter.fillRect(column * cellSize, row * cellSize, cellSize, cellSize, Qt::white);
                painter.drawRect(column * cellSize, row * cellSize, cellSize, cellSize);
            }
        }
    }
}

void TetrisGrid::drawShape(QPainter &painter, int cellSize) {
    for (int i = 0; i < currentShape_.size(); ++i) {
        for (int j = 0; j < currentShape_[i].size(); ++j) {
            if (currentShape_[i][j] == 1) {
                QColor color = generateRandomColor();
                QBrush brush(color);
                painter.setBrush(brush);
                painter.fillRect((shapeColumn_ + j) * cellSize, (shapeRow_ + i) * cellSize, cellSize, cellSize, color);
                painter.drawRect((shapeColumn_ + j) * cellSize, (shapeRow_ + i) * cellSize, cellSize, cellSize);
            }
        }
    }
}

void TetrisGrid::drawNextShape(QPainter &painter, int cellSize) {
    for (int i = 0; i < nextShape_.size(); ++i) {
        for (int j = 0; j < nextShape_[i].size(); ++j) {
            if (nextShape_[i][j] == 1) {
                QColor color = Qt::red;
                QBrush brush(color);
                painter.setBrush(brush);
                painter.fillRect((4 + j) * cellSize, (i) * cellSize, cellSize, cellSize, color);
                painter.drawRect((4 + j) * cellSize, (i) * cellSize, cellSize, cellSize);
            }
        }
    }
}

QColor TetrisGrid::generateRandomColor() {
    return QColor(rand() % 256, rand() % 256, rand() % 256);
}

void TetrisGrid::generateNewShape() {
    shapeRow_ = 4;
    shapeColumn_ = mColumns_ / 2 - 1;
    int type = rand() % 6;
    currentShape_.clear();
    currentShape_ = nextShape_;
    nextShape_.clear();
    if (type == 0) {
        nextShape_.append({1, 1});
        nextShape_.append({1, 1});
    } else if (type == 1) {
        nextShape_.append({1, 1, 1, 1});
    } else if (type == 2){
        nextShape_.append({1, 1});
        nextShape_.append({1, 0});
        nextShape_.append({1, 0});
    } else if (type == 3){
        nextShape_.append({1, 1});
    } else if (type == 4){
        nextShape_.append({1, 1});
        nextShape_.append({0, 1});
        nextShape_.append({0, 1});
    } else {
        nextShape_.append({0, 1, 0});
        nextShape_.append({1, 1, 1});
    }
    if (currentShape_.size() == 0) {
        generateNewShape();
    }
    timer_->setInterval(speed_);
}

void TetrisGrid::handleKeyPressEvent(QKeyEvent *event) {
    keyPressEvent(event);
}

void TetrisGrid::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        moveShapeLeft();
        break;
    case Qt::Key_Right:
        moveShapeRight();
        break;
    case Qt::Key_Up:
        rotateShape();
        break;
    case Qt::Key_Down:
        speedFigure();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

bool TetrisGrid::isCollision() {
    for (int i = 0; i < currentShape_.size(); ++i) {
        for (int j = 0; j < currentShape_[i].size(); ++j) {
            if (currentShape_[i][j] == 1) {
                int gridRow = shapeRow_ + i;
                int gridColumn = shapeColumn_ + j;
                if (gridRow < 0 || gridRow >= mRows_ || gridColumn < 0 || gridColumn >= mColumns_ || mGrid_[gridRow][gridColumn] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

void TetrisGrid::checkLines() {

    for (int row = mRows_ - 1; row >= 0; --row) {
        bool isLineFilled = true;
        for (int column = 0; column < mColumns_; ++column) {
            if (mGrid_[row][column] == 0) {
                isLineFilled = false;
                break;
            }
        }
        if (isLineFilled) {
            for (int r = row; r > 0; --r) {
                mGrid_[r] = mGrid_[r - 1];
            }
            mGrid_[0].fill(0);
            ++score_;
            updateScore(score_);
        }
    }
}
