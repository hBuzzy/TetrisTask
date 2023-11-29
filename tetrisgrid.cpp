#include "tetrisgrid.h"
#include <QPainter>
#include <QKeyEvent>
#include <QBrush>
#include <QMessageBox>

TetrisGrid::TetrisGrid(QWidget *parent)
    : QWidget(parent), m_rows_(24), m_cols_(10), timer_(new QTimer(this)), score_(0) {
    m_grid_.resize(m_rows_, QVector<int>(m_cols_, 0));
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
                int gridCol = shapeCol_ + j;
                if (gridRow >= 0 && gridRow < m_rows_ && gridCol >= 0 && gridCol < m_cols_) {
                    if (gridRow <= 0) {
                        emit gameOver();
                    }
                    else
                    m_grid_[gridRow][gridCol] = 1;
                }
            }
        }
    }
}

void TetrisGrid::setupGame() {
    m_grid_.clear();
    m_grid_.resize(m_rows_, QVector<int>(m_cols_, 0));
    score_ = 0;
    updateScore(score_);
    generateNewShape();
}

void TetrisGrid::speedFigure(){
    timer_->setInterval(100);
    timer_->start();
}

int TetrisGrid::rows() const {
    return m_rows_;
}

int TetrisGrid::cols() const {
    return m_cols_;
}

void TetrisGrid::setRows(int rows) {
    if (m_rows_ != rows) {
        m_rows_ = rows;
        m_grid_.resize(m_rows_);
        for (auto &row : m_grid_) {
            row.resize(m_cols_);
        }
        emit rowsChanged(m_rows_);
        update();
    }
}

void TetrisGrid::setCols(int cols) {
    if (m_cols_ != cols) {
        m_cols_ = cols;
        for (auto &row_ : m_grid_) {
            row_.resize(m_cols_);
        }
        emit colsChanged(m_cols_);
        update();
    }
}

void TetrisGrid::startGame() {
    setupGame();
    timer_->start();
}


void TetrisGrid::moveShapeLeft() {
    if (shapeCol_ > 0) {
        --shapeCol_;
        if (!isValidMove(currentShape_, shapeRow_, shapeCol_)) {
            ++shapeCol_;
        }
        update();
    }
}

void TetrisGrid::moveShapeRight() {
    if (shapeCol_ + currentShape_[0].size() < m_cols_) {
        ++shapeCol_;
        if (!isValidMove(currentShape_, shapeRow_, shapeCol_)) {
            --shapeCol_;
        }
        update();
    }
}

bool TetrisGrid::isValidMove(const QVector<QVector<int>> &shape, int rows, int cols) {
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] == 1) {
                int gridRow = rows + i;
                int gridCol = cols + j;
                if (gridRow < 0 || gridRow >= m_rows_ || gridCol < 0 || gridCol >= m_cols_) {
                    return false;
                }
                if (m_grid_[gridRow][gridCol] == 1) {
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
    QVector<int> temp;
    for (int j = currentShape_[0].size()-1; j >= 0; --j) {
        temp.clear();
        for (int i = 0; i < currentShape_.size(); ++i) {
            temp.append(currentShape_[i][j]);
        }
        rotatedShape.append(temp);
    }
    if (isValidMove(rotatedShape, shapeRow_, shapeCol_)) {
        currentShape_ = rotatedShape;
        update();
    }
}

void TetrisGrid::dropShape() {
        if (shapeRow_ + currentShape_.size() <= m_rows_) {
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
    int cellSize = qMin(width() / m_cols_, height() / m_rows_);
    drawGrid(painter, cellSize);
    drawShape(painter, cellSize);
    drawNextShape(painter, cellSize);
}

void TetrisGrid::drawGrid(QPainter &painter, int cellSize) {
    for (int row = 0; row < m_rows_; ++row) {
        for (int col = 0; col < m_cols_; ++col) {
            if (m_grid_[row][col] == 1) {
                QColor color = generateRandomColor();
                painter.fillRect(col * cellSize, row * cellSize, cellSize, cellSize, color);
            } else {
                painter.fillRect(col * cellSize, row * cellSize, cellSize, cellSize, Qt::white);
                painter.drawRect(col * cellSize, row * cellSize, cellSize, cellSize);
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
                painter.fillRect((shapeCol_ + j) * cellSize, (shapeRow_ + i) * cellSize, cellSize, cellSize, color);
                painter.drawRect((shapeCol_ + j) * cellSize, (shapeRow_ + i) * cellSize, cellSize, cellSize);
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
    shapeCol_ = m_cols_ / 2 - 1;
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
        emit moveShapeLeft();
        break;
    case Qt::Key_Right:
        emit moveShapeRight();
        break;
    case Qt::Key_Up:
        emit rotateShape();
        break;
    case Qt::Key_Down:
        emit speedFigure();
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
                int gridCol = shapeCol_ + j;
                if (gridRow < 0 || gridRow >= m_rows_ || gridCol < 0 || gridCol >= m_cols_ || m_grid_[gridRow][gridCol] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

void TetrisGrid::checkLines() {

    for (int row = m_rows_ - 1; row >= 0; --row) {
        bool lineFilled = true;
        for (int col = 0; col < m_cols_; ++col) {
            if (m_grid_[row][col] == 0) {
                lineFilled = false;
                break;
            }
        }
        if (lineFilled) {
            for (int r = row; r > 0; --r) {
                m_grid_[r] = m_grid_[r - 1];
            }
            m_grid_[0].fill(0);
            ++score_;
            updateScore(score_);
        }
    }
}
